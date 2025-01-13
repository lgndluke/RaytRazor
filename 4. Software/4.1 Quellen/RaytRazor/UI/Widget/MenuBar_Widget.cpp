#include "MenuBar_Widget.h"

#include "../../Parsing/Json_Parser.h"

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
                    std::string path_to_file = openFileDialog();
                    printf("%s\n", path_to_file.c_str());
                },
                []() {
                    std::string path_to_DIR = openFileDialog();
                    if(isDirectory(path_to_DIR)) return;
                    if(Json_Parser::exportToJSON(path_to_DIR)) Logger::log(MessageType::INFO, "Successfully exported to JSON");

                },
                []() { nanogui::shutdown(); }
            });

    // Add-Menü hinzufügen
    addMenu("Add",
            {"Object", "Light", "Camera"},
            {
                []() { std::cout << "Add Object clicked!" << std::endl; },
                []() { std::cout << "Add Light clicked!" << std::endl; },
                []() { std::cout << "Add Camera clicked!" << std::endl; }
            });

    // Help-Menü hinzufügen
    addMenu("Help",
            {"Documentation", "About"},
            {
                []() { std::cout << "Documentation clicked!" << std::endl; },
                []() { std::cout << "About clicked!" << std::endl; }
            });
}

void MenuBar_Widget::addMenu(const string& title, const vector<string>& options, const vector<function<void()>>& callbacks) {
    auto menuButton = new PopupButton(this, title);
    menuButton->setChevronIcon(ENTYPO_ICON_CHEVRON_DOWN);
    menuButton->setFixedHeight(25);

    Popup* popup = menuButton->popup();
    popup->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 5, 5));

    menuButton->setCallback([menuButton, popup]() {
        popup->setAnchorPos(menuButton->position() + Eigen::Vector2i(0, menuButton->size().y() + 27));
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


std::string MenuBar_Widget::openFileDialog() {
    char filePath[MAX_PATH] = {100};

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Zeige den Dialog und prüfe, ob der Benutzer eine Datei ausgewählt hat
    if (GetOpenFileName(&ofn)) {
        return string(filePath);
    } else {
        return "";
    }
}

bool MenuBar_Widget::isDirectory(const std::string& path) {
    return is_directory(fs::path(path)); // Überprüft, ob der Pfad ein Verzeichnis ist
}