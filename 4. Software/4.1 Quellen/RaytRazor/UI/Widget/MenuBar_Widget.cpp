#include <shlobj.h>
#include "MenuBar_Widget.h"


MenuBar_Widget::MenuBar_Widget(Widget* parent) : Widget(parent) {
    setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 10));
    setFixedHeight(30);
}

void MenuBar_Widget::initialize() {
    // File-Menü hinzufügen
    addMenu("File",
            {"Open", "Save", "Exit"},
            {
                []() {
                    Main_Scene::openScene();
                },
                []() {
                    std::string path_to_DIR = openDirectoryDialog();
                    if(!isDirectory(path_to_DIR)) return;
                    if(Json_Parser::exportToJSON(path_to_DIR, Main_Scene::getComponents(), Main_Scene::getResources()))
                        Logger::log(MessageType::INFO, "Successfully exported to JSON");
                },
                []() { exit(0); }
            });

    // Add-Menü hinzufügen
    addMenu("Add",
            {"Object", "Light", "Camera"},
            {
                []() {
                    boost::uuids::uuid uuid = boost::uuids::random_generator()();
                    boost::uuids::uuid mat_uuid = boost::uuids::random_generator()();
                    boost::uuids::uuid obj_uuid = boost::uuids::random_generator()();

                    std::string path_to_file = openFileDialog();
                    std::string path_to_file_mtl = openFileDialog();

                    std::optional<Object_Resource> OR = Object_Importer::import_Object(obj_uuid,path_to_file);
                    std::optional<Material_Resource> MR = Material_Importer::import_Material(mat_uuid,path_to_file_mtl);

                    if (!OR.has_value() || !MR.has_value()) return;

                    auto render_comp = std::make_shared<Render_Component>(
                        uuid,
                        "Render_Added",
                        glm::vec3{0, 0, 0},
                        glm::vec3{0, 0, 0},
                        glm::vec3{1, 1, 1},
                        obj_uuid,
                        mat_uuid
                    );

                    auto object_resource = std::make_shared<Object_Resource>(OR.value());
                    auto material_resource = std::make_shared<Material_Resource>(MR.value());

                    Main_Scene::addResource(mat_uuid, material_resource);
                    Main_Scene::addResource(obj_uuid, object_resource);
                    Main_Scene::addComponent(uuid, render_comp);
                },
                [](){
                        boost::uuids::uuid uuid = boost::uuids::random_generator()();
                        auto light_comp = std::make_shared<Light_Component>(
                            uuid,
                            "Light_Added",
                            glm::vec3{0, 10, 0},
                            glm::vec3{0, 0, 0},
                            glm::vec3{1, 1, 1},
                            1.0f,
                            glm::vec3{255, 255, 255}
                        );

                        Main_Scene::addComponent(uuid, light_comp);
                    },
                []() {
                        boost::uuids::uuid uuid = boost::uuids::random_generator()();
                        auto camera_comp = std::make_shared<Camera_Component>(
                            uuid,
                            "Camera_Added",
                            glm::vec3{0, 65, 100},
                            glm::vec3{-35, 0, 0},
                            glm::vec3{1, 1, 1},
                            60,
                            1.77f,
                            0.1f,
                            1000
                        );

                        Main_Scene::addComponent(uuid, camera_comp);
                }
            });

    // Help-Menü hinzufügen
    addMenu("Help",
            {"Documentation", "About"},
            {
                []() { ShellExecuteA(nullptr, "open", "https://github.com/lgndluke/RaytRazor/wiki/", nullptr, nullptr, SW_SHOWNORMAL); },
                []() { ShellExecuteA(nullptr, "open", "https://github.com/lgndluke/RaytRazor/", nullptr, nullptr, SW_SHOWNORMAL); }
            });
}

void MenuBar_Widget::addMenu(const string& title, const vector<string>& options, const vector<function<void()>>& callbacks) {
    auto menuButton = new PopupButton(this, title);
    menuButton->setChevronIcon(ENTYPO_ICON_CHEVRON_DOWN);
    menuButton->setFixedHeight(25);

    Popup* popup = menuButton->popup();
    popup->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 5, 5));
    popup->setAnchorPos(menuButton->position() + Eigen::Vector2i(0, menuButton->size().y() + 27));
    popup->setVisible(false);

    auto updatePopupPosition = [menuButton, popup]() {
        popup->setAnchorPos(menuButton->position() + Eigen::Vector2i(0, menuButton->size().y() + 27));
    };

    menuButton->setCallback([popup, updatePopupPosition]() {
        updatePopupPosition();
        popup->setVisible(!popup->visible());
    });

    for (size_t i = 0; i < options.size(); ++i) {
        auto optionButton = new Button(popup, options[i]);
        optionButton->setFixedHeight(25);
        if (i < callbacks.size() && callbacks[i]) {
            optionButton->setCallback(callbacks[i]);
        }
    }
}

void MenuBar_Widget::performLayout(NVGcontext* ctx) {
    Widget::performLayout(ctx);

    for (auto& child : children()) {
        if (auto menuButton = dynamic_cast<PopupButton*>(child)) {
            if (auto popup = menuButton->popup()) {
                popup->setAnchorPos(menuButton->position() + Eigen::Vector2i(0, menuButton->size().y() + 27));
            }
        }
    }
}



std::string MenuBar_Widget::openFileDialog() {
    char filePath[MAX_PATH] = {100};

    path currentPath = current_path();

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    std::string result;
    if (GetOpenFileName(&ofn)) {
        result = filePath;
        std::replace(result.begin(), result.end(), '\\', '/');
    }

    current_path(currentPath);

    return result;

}

std::string MenuBar_Widget::openDirectoryDialog() {
    BROWSEINFO bi = {0};

    path currentPath = current_path();

    bi.lpszTitle = "Select a Directory";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != nullptr) {
        char path[MAX_PATH];
        if (SHGetPathFromIDList(pidl, path)) {
            std::string directoryPath = path;
            std::replace(directoryPath.begin(), directoryPath.end(), '\\', '/'); // Ersetze Backslashes
            CoTaskMemFree(pidl); // Speicher freigeben

            current_path(currentPath);
            return directoryPath;
        }
        CoTaskMemFree(pidl); // Speicher freigeben
    }

    current_path(currentPath);
    return ""; // Rückgabe eines leeren Strings, wenn kein Verzeichnis ausgewählt wurde
}

bool MenuBar_Widget::isDirectory(const std::string& path) {
    return is_directory(fs::path(path)); // Überprüft, ob der Pfad ein Verzeichnis ist
}