#include "Main_Scene.h"

std::map<boost::uuids::uuid, std::shared_ptr<Base_Component>> Main_Scene::components;
std::map<boost::uuids::uuid, std::shared_ptr<Base_Resource>> Main_Scene::resources;
Main_Scene* Main_Scene::instance = nullptr;

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

    if (!initialized)
    {
        mShader.init(
            "3d_Preview_Shader",
            Vertex_Shader::get_vertex_shader(),
            Fragment_Shader::get_fragment_shader()
        );
        initialized = true;
    }

    //clear screen and setzt hintergrund grau
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Shader binden und Szene rendern
    mShader.bind();

    Eigen::Matrix4f mvpEigen;
    mvpEigen.setIdentity();
    glm::mat4 mvpGLM, projGLMmat, viewGLMmat;
    auto components = Main_Scene::getComponents();
    auto resources = Main_Scene::getResources();
    //create projection and view matrix
    for (auto& pair  : components)
    {
        shared_ptr<Camera_Component> camera = dynamic_pointer_cast<Camera_Component>(pair.second);
        if(camera)
        {
            std::pair<glm::vec3, glm::vec3> cameraPair = calculateCameraVectors(camera->get_position(),camera->get_rotation());
            viewGLMmat = glm::lookAt(camera->get_position(), cameraPair.first, cameraPair.second);
            projGLMmat = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near_clip(), camera->get_far_clip());
        }
    }

    //iterate through components and bind the new mvpEigen matrix/object -> get model matrix, calculate matrix, bind new mvpEigen, print screen
    //model matrix
    for (auto& pair : components)
    {
        shared_ptr<Render_Component> render = dynamic_pointer_cast<Render_Component>(pair.second);
        if(render)
        {
            // Validate Object Resource
            auto objIt = resources.find(render->get_object_UUID());
            if (objIt == resources.end()) {
                std::cerr << "Object UUID not found in resources!" << std::endl;
                continue;
            }

            shared_ptr<Object_Resource> objRes = dynamic_pointer_cast<Object_Resource>(objIt->second);
            if (!objRes) {
                std::cerr << "Failed to cast to Object_Resource!" << std::endl;
                continue;
            }

            // Validate Material Resource
            auto matIt = resources.find(render->get_material_UUID());
            if (matIt == resources.end()) {
                std::cerr << "Material UUID not found in resources!" << std::endl;
                continue;
            }

            shared_ptr<Material_Resource> matRes = dynamic_pointer_cast<Material_Resource>(matIt->second);
            if (!matRes) {
                std::cerr << "Failed to cast to Material_Resource!" << std::endl;
                continue;
            }
            //calculate Model Matrix in GLM
            glm::mat4 modelGLMmat = extract_Model_Matrix(render);

            //calculate the new mvpGLM matrix
            mvpGLM = projGLMmat * viewGLMmat * modelGLMmat;

            //convert mvpGLM to eigen mvpEigen
            mvpEigen = Converter::convert_from_GLM_to_EigenMatrix(mvpGLM);
            Converter::convert_to_matrix_indices(objRes);
            Converter::convert_to_matrix_vertices(objRes);
            Converter::convert_to_matrix_colors(matRes);

            std::cout << "Object Resource Indices:\n" << objRes->get_matrix_indices() << std::endl;
            std::cout << "Object Resource Vertices:\n" << objRes->get_matrix_vertices() << std::endl;

            // Bind Indices, Colors and Vertices
            mShader.uploadIndices(objRes->get_matrix_indices());
            mShader.uploadAttrib("position", objRes->get_matrix_vertices());
            mShader.uploadAttrib("color", matRes->get_matrix_colors());

            //bind mvpEigen and draw the component
            mShader.setUniform("modelViewProj", mvpEigen);
            glEnable(GL_DEPTH_TEST);
            mShader.drawIndexed(GL_TRIANGLES, 0, objRes->get_matrix_indices().size());
            glDisable(GL_DEPTH_TEST);
        }
    }
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
    const int component_tree_width          = this->window_width * 0.33f;
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

    // Setze ein Layout für das Fenster
    component_tree->setLayout(new BoxLayout(
        Orientation::Vertical, Alignment::Fill, 10, 10
    ));

    const auto component_attributes = new Fixed_Window(this, "Component Attributes");
    component_attributes->setPosition(Eigen::Vector2i(component_attributes_position_x, component_attributes_position_y));
    component_attributes->setSize(Eigen::Vector2i(component_attributes_width, component_attributes_height));

    attributesWidget = new ComponentAttributes_Widget(component_attributes);

    tree_view = new TreeView_Widget(component_tree, attributesWidget);
    tree_view->setPosition(Eigen::Vector2i(component_tree_position_x, component_tree_position_y + 30));
    tree_view->setSize(Eigen::Vector2i(component_tree_width, component_tree_height - 50));

    const auto preview_canvas = new Preview_Canvas(preview_window);
    preview_window->addChild(preview_canvas);
    preview_canvas->setPosition({ preview_position_x + 10, preview_position_y + 35 });
    preview_canvas->setSize({preview_width - 20, preview_height - 80});

    const auto raytrace_preview_button = new Button(preview_window, "Raytrace Preview");
    preview_window->addChild(raytrace_preview_button);
    raytrace_preview_button->setCallback([this]
    {
        try
        {
            pthread_t SDL_thread;
            pthread_create(&SDL_thread, NULL, raytrace_preview(), NULL);
        }
        catch(...)
        {
            // TODO: Error Handling.
        }
    });
    raytrace_preview_button->setSize({(preview_width / 2) - 20, 30});
    raytrace_preview_button->setPosition({preview_position_x + raytrace_preview_button->width() + 25, preview_height - 40});

    const auto load_json_button = new Button(preview_window, "Import Scene");
    preview_window->addChild(load_json_button);
    load_json_button->setCallback([this]
    {
        try
        {
            Logger::log(MessageType::INFO, "Main_Scene::initialize() - Import Scene Button clicked.");

            //Path muss angepasst werden wenn sich wd die Struktur ändert
            //todo über explorer setzen :)
            string path_to_json = "C:/Users/lukas/OneDrive - thu.de/5. Semester/Software Projekt/RaytRazor/4. Software/4.1 Quellen/RaytRazor/Parsing/Dummy_Json_New.json";

            //string path_to_json1 = openFileDialog();
            if (path_to_json.empty()) return;
            if (!isJsonFileAndFixPath(path_to_json)) return;
            if (!exists(path_to_json)) {
                std::cerr << "File does not exist: " << path_to_json << std::endl;
                return;
            }


            path_to_json = absolute(path_to_json).string();

            printf("");


            //FileSelector file_Selector();
            //string path_to_json = file_Selector().get_input_path();

            components.clear();
            resources.clear();

            Json_Parser::parseJSON(path_to_json, components, resources);

            printf("");

            tree_view->clear();
            tree_view->addNode("3D-Szene");

            if (components.empty()) {
                printf("No components to display.\n");
                return;
            }

            // Zeige die Attribute des ersten Elements an
            bool isFirstComponent = true;

            for (const auto& [key, component] : components) {
                if (isFirstComponent) {
                    attributesWidget->showAttributesOfComponent(component);
                    isFirstComponent = false;
                }

                // Füge den Knoten zum Baum hinzu
                tree_view->addNode(component->get_name(), "3D-Szene");
            }

            performLayout();

        }
        catch(...)
        {
            // TODO: Error Handling.
        }
    });
    load_json_button->setSize({(preview_width / 2) - 20, 30});
    load_json_button->setPosition({ preview_position_x + 15, preview_height - 40});

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
    bool isFirstComponent = true;

    for (const auto& [key, component] : components) {
        if (isFirstComponent) {
            attributesWidget->showAttributesOfComponent(component);
            isFirstComponent = false;
        }

        // Füge den Knoten zum Baum hinzu
        tree_view->addNode(component->get_name(), "3D-Szene");
    }
}


