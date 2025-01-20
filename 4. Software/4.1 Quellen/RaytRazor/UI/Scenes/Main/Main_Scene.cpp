#include "Main_Scene.h"



//TODO's:
// -> Preview_Canvas::drawGL() implementieren.
// -> Main_Scene::Main_Scene() implementieren.
// -> Main_Scene::update() implementieren.

std::map<boost::uuids::uuid, std::shared_ptr<Base_Component>> Main_Scene::components;
std::map<boost::uuids::uuid, std::shared_ptr<Base_Resource>> Main_Scene::resources;
Main_Scene* Main_Scene::instance = nullptr;
float Main_Scene::scaling = 0.5f;

Fixed_Window::Fixed_Window(Widget* parent, const std::string& title)
                           : Window(parent, title)
{}

bool Fixed_Window::mouseDragEvent(const Vector2i& p, const Vector2i &rel,
                                  int button, int /* modifiers */)
{
    return false;
}

Preview_Canvas::Preview_Canvas(Widget* parent) : GLCanvas(parent)
{}

void Preview_Canvas::drawGL()
{

    static bool initialized = false;


    //gesamte scene muss hier vorliegen
    boost::uuids::uuid uuid = boost::uuids::random_generator()();

    //C:\Users\chris\CLionProjects\RaytRazor\RaytRazor\5. Modelle\5.1 Beispielmodelle\miscellaneous\miscellaneous\teapot\Teapot.obj
    //C:/Users/lukas/OneDrive - thu.de/5. Semester/Software Projekt/RaytRazor/5. Modelle/5.1 Beispielmodelle/miscellaneous/miscellaneous/teapot/Teapot.mtl
    Object_Resource OR;
    Material_Resource MR;
    try
    {
        OR = Object_Importer::import_Object(uuid,
            "C:/Users/blau08/OneDrive - thu.de/Semester 5/Software Projekt/RaytRazor/RaytRazor5. Modelle/5.1 Beispielmodelle/miscellaneous/miscellaneous/teapot/Teapot.obj").value();
        MR = Material_Importer::import_Material(uuid,
            "C:/Users/blau08/OneDrive - thu.de/Semester 5/Software Projekt/RaytRazor/RaytRazor5. Modelle/5.1 Beispielmodelle/miscellaneous/miscellaneous/teapot/Teapot.mtl").value();
    }
    catch (const std::exception& e)
    {
        Logger::log(MessageType::SEVERE, "Main_Scene::drawGL: File not found exception:\n" + std::string(e.what()));
    }

    if (!initialized)
    {

        // Initialisierung des Shaders.
        mShader.init(
            "3d_Preview_Shader",
            Vertex_Shader::get_vertex_shader(),
            Fragment_Shader::get_fragment_shader()
        );
        initialized = true;

        // TODO Components in 3D Preview laden.

        // Dummy Daten -> TODO Delete afterwards.
        // ==================================================================


        const std::vector<Vertex>& vertices = OR.get_vertices(); // Optimierter Zugriff durch Referenz
        const std::vector<Indice>& indices = OR.get_indices(); // Zugriff auf Indizes

        int size = indices.size();
        int size2 = vertices.size();

        /*
        // Ausgabe der Vertices
        for (int i = 0; i < static_cast<int>(vertices.size()); i++) {
            const Vertex& vertex = vertices[i];

            if (i >= 0) {
                std::cout << "Vertex " << i << ": "
                          << "Position = (" << vertex.position.x << ", "
                                            << vertex.position.y << ", "
                                            << vertex.position.z << "), "
                          << "Color = (" << vertex.color.r << ", "
                                         << vertex.color.g << ", "
                                         << vertex.color.b << ")" << std::endl;
            }
        }

        // Ausgabe der Indizes
        if (!indices.empty() && indices.size() % 3 == 0) {
            for (size_t i = 0; i < indices.size() / 3; i++) {
                std::cout << "Face " << i << ": "
                          << "Indices = (" << indices[i * 3] << ", "
                                           << indices[i * 3 + 1] << ", "
                                           << indices[i * 3 + 2] << ")" << std::endl;
            }
        } else {
            std::cerr << "Warning: Indices vector is empty or not divisible by 3!" << std::endl;
        }
        */

        //Converter::convert_to_matrix_colors(MR);
        //Converter::convert_to_matrix_indices(OR);
        //Converter::convert_to_matrix_vertices(OR);
        // ==================================================================

        printf("");

        // Daten in Shader laden.
        mShader.bind();
        mShader.uploadIndices(OR.get_matrix_indices());

        printf("");
        mShader.uploadAttrib("position", OR.get_matrix_vertices());
        mShader.uploadAttrib("color", MR.get_matrix_colors());
    }

    // Shader binden und Szene rendern
    mShader.bind();

    Eigen::Matrix4f mvp;
    mvp.setIdentity();
    glm::mat4 mvpGLM;
    map<boost::uuids::uuid, shared_ptr<Base_Component>> components;
    components = Main_Scene::getComponents();
    map<boost::uuids::uuid, shared_ptr<Base_Resource>> resources;
    resources = Main_Scene::getResources();
    //create projection and view matrix
    for (auto& pair  : components) {

        shared_ptr<Camera_Component> camera = dynamic_pointer_cast<Camera_Component>(pair.second);

        //std::pair<glm::vec3, glm::vec3> cameraPair = calculateCameraVectors(camera->get_position(),camera->get_rotation());
        //glm::mat4 ViewGLMmat = glm::lookAt(camera->get_position(), cameraPair.first, cameraPair.second);
        //glm::mat4 ProjGLMmat = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(),
                                                        //camera->get_near_clip(), camera->get_far_clip());
    }

    //iterate through components and bind the new mvp matrix/object -> get model matrix, calculate matrix, bind new mvp, print screen
    //model matrix

    //convert mvpGLM -> mvp von eigen


    mShader.setUniform("modelViewProj", mvp);

    glEnable(GL_DEPTH_TEST);
    mShader.drawIndexed(GL_TRIANGLES, 0, OR.get_indices().size());
    glDisable(GL_DEPTH_TEST);
}

glm::mat4 Preview_Canvas::calculateViewDir(const glm::vec3 rotation)
{
    const float pitch = glm::radians(rotation.x);
    const float yaw   = glm::radians(rotation.y);
    const float roll  = glm::radians(rotation.z);

    const glm::mat4 rotation_x = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 rotation_y = glm::rotate(glm::mat4(1.0f), yaw,   glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 rotation_z = glm::rotate(glm::mat4(1.0f), roll,  glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::mat4 result = rotation_x * rotation_y * rotation_z;

    return result;
}

std::pair<glm::vec3, glm::vec3> Preview_Canvas::calculateCameraVectors(const glm::vec3 position, const glm::vec3 rotation)
{
    const glm::mat4 directions = calculateViewDir(rotation);

    constexpr glm::vec3 base_forward(0.0f, 0.0f, -1.0f);
    constexpr glm::vec3 base_up     (0.0f, 1.0f, 0.0f);

    const glm::vec3 forward = glm::normalize(glm::vec3(directions * glm::vec4(base_forward, 0.0f)));
    glm::vec3 up            = glm::normalize(glm::vec3(directions * glm::vec4(base_up, 0.0f)));

    return { position + forward, up };
}

glm::mat4 Preview_Canvas::extract_Model_Matrix(const shared_ptr<Render_Component>& input) {
    auto result = glm::mat4(1.0f);
    result = glm::translate(result, input->get_position());
    result = glm::rotate(result, glm::radians(input->get_rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    result = glm::rotate(result, glm::radians(input->get_rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    result = glm::rotate(result, glm::radians(input->get_rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    result = glm::scale(result, input->get_scale());
    return result;
}

Main_Scene::Main_Scene(const int window_width,
                       const int window_height,
                       const string& window_title,
                       const bool is_resizeable)
                       : Screen(Eigen::Vector2i(window_width, window_height), window_title, is_resizeable)
{
    this->window_width = window_width;
    this->window_height = window_height;
    this->window_title = window_title;
    this->is_resizeable = is_resizeable;
    instance = this;

    this->ids = vector<int>();
    this->components = map<boost::uuids::uuid, shared_ptr<Base_Component>>();
    this->resources = map<boost::uuids::uuid, shared_ptr<Base_Resource>>();

    initialize();
}

void Main_Scene::forceUpdate() {
    if (instance) {
        instance->updateTreeView();
        instance->update();
    }
}

void Main_Scene::initialize()
{
    constexpr int preview_position_x = 0;
    constexpr int preview_position_y = 0;
    const int preview_width          = this->window_width * 0.67;
    const int preview_height         = this->window_height;

    const int component_tree_position_x     = preview_width;
    constexpr int component_tree_position_y = 0;
    const int component_tree_width          = this->window_width * 0.32f;
    const int component_tree_height         = this->window_height * 0.4f;

    const int component_attributes_position_x = preview_width;
    const int component_attributes_position_y = component_tree_height;
    const int component_attributes_width = this->window_width * 0.33f;
    const int component_attributes_height = this->window_height * 0.6f;

    const auto preview_window = new Fixed_Window(this, "3D-Preview");
    preview_window->setPosition(Eigen::Vector2i(preview_position_x, preview_position_y));
    preview_window->setSize(Eigen::Vector2i(preview_width, preview_height));

    auto menu_bar = new MenuBar_Widget(preview_window);
    menu_bar->initialize();

    const auto component_tree = new Fixed_Window(this, "Component Tree");
    component_tree->setPosition(Eigen::Vector2i(component_tree_position_x, component_tree_position_y));
    component_tree->setSize(Vector2i(component_tree_width, component_tree_height));

    // Setze ein Layout für das Fenster
    component_tree->setLayout(new BoxLayout(
        Orientation::Vertical, Alignment::Fill, 10, 10
    ));

    const auto component_attributes = new Fixed_Window(this, "Component Attributes");
    component_attributes->setPosition(Eigen::Vector2i(component_attributes_position_x, component_attributes_position_y));
    component_attributes->setSize(Eigen::Vector2i(component_attributes_width, component_attributes_height));

    attributesWidget = new ComponentAttributes_Widget(component_attributes);
    attributesWidget->showAttributesOfComponent();

    tree_view = new TreeView_Widget(component_tree, attributesWidget);
    tree_view->setPosition(Eigen::Vector2i(component_tree_position_x, component_tree_position_y + 30));
    tree_view->setSize(Eigen::Vector2i(component_tree_width, component_tree_height - 50));

    const auto preview_canvas = new Preview_Canvas(preview_window);
    preview_window->addChild(preview_canvas);
    preview_canvas->setPosition({ preview_position_x + 10, preview_position_y + 35 });
    preview_canvas->setSize({preview_width - 20, preview_height - 80});

    const auto raytrace_preview_button = new Button(preview_window, "Raytrace Preview");
    raytrace_preview_button->setFixedSize({preview_width / 2 - 10, 30}); // Breite etwas reduzieren für Platz
    preview_window->addChild(raytrace_preview_button);
    raytrace_preview_button->setCallback([this] {
        try {
            pthread_t SDL_thread;
            pthread_create(&SDL_thread, NULL, raytrace_preview(), NULL);
        } catch (...) {
            // TODO: Error Handling.
        }
    });
    raytrace_preview_button->setPosition({10, preview_height - 40});

    auto *slider = new Slider(preview_window);
    slider->setValue(0.5f);
    slider->setFixedSize({preview_width / 2 - 50, 30});
    preview_window->addChild(slider);

    slider->setPosition(
        {preview_width / 2 + 50,
         preview_height - 40});

    slider->setCallback([](const float value) {
        scaling = value;
    });


    auto scaling_label = new Label(preview_window, "Scaler:");
    scaling_label->setPosition({preview_width / 2 + 10, preview_height - 35});
    scaling_label->setFixedSize({50,20});
    scaling_label->setFontSize(20);


    performLayout();
}

void*(*Main_Scene::raytrace_preview())(void*)
{
    CApp app;
    app.OnExecute();
    return nullptr;
}

void Main_Scene::update()
{
    performLayout();
}

bool Main_Scene::keyboardEvent(int key, int scancode, int action, int modifiers) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_L && modifiers == GLFW_MOD_CONTROL) {
            Logger::log(MessageType::INFO, "Shortcut: Save (Ctrl+S)");
            boost::uuids::uuid uuid = boost::uuids::random_generator()();
            auto light_comp = std::make_shared<Light_Component>(
                uuid,
                "Light_Added",
                glm::vec3{0, 0, 0},
                glm::vec3{0, 0, 0},
                glm::vec3{0, 0, 0},
                1.0f,
                glm::vec3{1, 1, 1}
            );
            addComponent(uuid, light_comp);
            return true;
        }
        if (key == GLFW_KEY_O && modifiers == GLFW_MOD_CONTROL) {
            Logger::log(MessageType::INFO, "Shortcut: Open (Ctrl+O)");
            openScene();
            return true;
        }
        if (key == GLFW_KEY_Q && modifiers == GLFW_MOD_CONTROL) {
            Logger::log(MessageType::INFO, "Shortcut: Quit (Ctrl+Q)");
            setVisible(false);
            return true;
        }
        if (key == GLFW_KEY_R && modifiers == GLFW_MOD_CONTROL) {
            Logger::log(MessageType::INFO, "Shortcut: Quit (Ctrl+Q)");
            setVisible(false);
            pthread_t SDL_thread;
            pthread_create(&SDL_thread, NULL, raytrace_preview(), NULL);
            return true;
        }
    }
    return Screen::keyboardEvent(key, scancode, action, modifiers);
}


std::string Main_Scene::openFileDialog() {
    char filePath[MAX_PATH] = {0};

    path currentPath = current_path();

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
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

void Main_Scene::addComponent(const boost::uuids::uuid& uuid, const std::shared_ptr<Base_Component>& component) {
    components[uuid] = component;
    forceUpdate();
}

bool Main_Scene::isJsonFileAndFixPath(std::string& path) {
    // Check if the file has a .json extension (case insensitive)
    const std::string extension = ".json";
    if (path.size() >= extension.size() &&
        std::equal(extension.rbegin(), extension.rend(), path.rbegin(), [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
        })) {
        // Replace all backslashes with forward slashes
        std::replace(path.begin(), path.end(), '\\', '/');
        return true;
        }
    return false;
}

void Main_Scene::updateTreeView() const {
    tree_view->clear();
    //tree_view->addNode("3D-Szene");
    instance->tree_view->addParent("3D-Szene");

    for (const auto& [key, component] : components) {
        tree_view->addNode(component, "3D-Szene");
    }
}

void Main_Scene::setChangesOnComponent(const std::shared_ptr<Base_Component>& component)
{
    for (auto& pair : getComponents()) {
        if (pair.second->get_uuid() == component->get_uuid()) {
            components[component->get_uuid()] = component;
            instance->attributesWidget->updateFromComponent(components[component->get_uuid()]);
            break;
        }
    }

    forceUpdate();
}

void Main_Scene::openScene()
{
    Logger::log(MessageType::INFO, "Main_Scene::initialize() - Import Scene Button clicked.");

    string path_to_json = openFileDialog();
    if (path_to_json.empty()) return;
    if (!isJsonFileAndFixPath(path_to_json)) return;
    if (!exists(path_to_json)) {
        std::cerr << "File does not exist: " << path_to_json << std::endl;
        return;
    }
    instance->scene_path = path_to_json;
    path_to_json = absolute(path_to_json).string();
    components.clear();
    resources.clear();

    Json_Parser::parseJSON(path_to_json, components, resources);

    instance->tree_view->clear();
    instance->tree_view->addParent("3D-Szene");

    if (components.empty()) {
        printf("No components to display.\n");
        return;
    }

    instance->attributesWidget->showAttributesOfComponent();

    for (const auto& [key, component] : components) {
        instance->tree_view->addNode(component, "3D-Szene");
    }

    instance->performLayout();
}

float Main_Scene::getScalingFactor()
{
    return scaling;
}
