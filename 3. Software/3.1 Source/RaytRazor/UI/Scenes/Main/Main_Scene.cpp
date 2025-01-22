#include "Main_Scene.h"

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

bool Preview_Canvas::keyboardEvent(int key, int scancode, int action, int modifiers)
{
    if (key == GLFW_KEY_W) {
        camera_glob->set_position({camera_glob->get_position().x, camera_glob->get_position().y, camera_glob->get_position().z - 10});
        Main_Scene::setChangesOnComponent(camera_glob);
        return true;
    }
    if (key == GLFW_KEY_S) {
        camera_glob->set_position({camera_glob->get_position().x, camera_glob->get_position().y, camera_glob->get_position().z + 10});
        Main_Scene::setChangesOnComponent(camera_glob);
        return true;
    }
    if (key == GLFW_KEY_A) {
        camera_glob->set_position({camera_glob->get_position().x - 10, camera_glob->get_position().y, camera_glob->get_position().z});
        Main_Scene::setChangesOnComponent(camera_glob);
        return true;
    }
    if (key == GLFW_KEY_D) {
        camera_glob->set_position({camera_glob->get_position().x + 10, camera_glob->get_position().y, camera_glob->get_position().z});
        Main_Scene::setChangesOnComponent(camera_glob);
        return true;
    }
    if (key == GLFW_KEY_E) {
        camera_glob->set_position({camera_glob->get_position().x, camera_glob->get_position().y + 10, camera_glob->get_position().z});
        Main_Scene::setChangesOnComponent(camera_glob);
        return true;
    }
    if (key == GLFW_KEY_Q) {
        camera_glob->set_position({camera_glob->get_position().x, camera_glob->get_position().y - 10, camera_glob->get_position().z});
        Main_Scene::setChangesOnComponent(camera_glob);
        return true;
    }
    if (key == GLFW_KEY_R) {
        camera_glob->set_rotation({camera_glob->get_rotation().x, camera_glob->get_rotation().y - 10, camera_glob->get_rotation().z});
        Main_Scene::setChangesOnComponent(camera_glob);
        return true;
    }
    if (key == GLFW_KEY_F) {
        camera_glob->set_rotation({camera_glob->get_rotation().x, camera_glob->get_rotation().y + 10, camera_glob->get_rotation().z});
        Main_Scene::setChangesOnComponent(camera_glob);
        return true;
    }

    return GLCanvas::keyboardEvent(key, scancode, action, modifiers);
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
            camera_glob = dynamic_pointer_cast<Camera_Component>(pair.second);
            std::pair<glm::vec3, glm::vec3> cameraPair = calculateCameraVectors(camera->get_position(),camera->get_rotation());
            viewGLMmat = glm::lookAt(camera->get_position(), cameraPair.first, cameraPair.second);
            projGLMmat = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near_clip(), camera->get_far_clip());
        }
    }
    for (auto& pair  : components)
    {
        shared_ptr<Render_Component> render = dynamic_pointer_cast<Render_Component>(pair.second);
        shared_ptr<Light_Component> light = dynamic_pointer_cast<Light_Component>(pair.second);

        if(render)
        {
            // Validate Object Resource
            auto objIt = resources.find(render->get_object_UUID());
            if (objIt == resources.end()) {
                Logger::log(MessageType::SEVERE,"Object UUID not found in resources!");
                continue;
            }

            shared_ptr<Object_Resource> objRes = dynamic_pointer_cast<Object_Resource>(objIt->second);
            if (!objRes) {
                Logger::log(MessageType::SEVERE,"Failed to cast to Object_Resource!");
                continue;
            }

            // Validate Material Resource
            auto matIt = resources.find(render->get_material_UUID());
            if (matIt == resources.end()) {
                 Logger::log(MessageType::SEVERE,"Material UUID not found in resources!");
                continue;
            }

            shared_ptr<Material_Resource> matRes = dynamic_pointer_cast<Material_Resource>(matIt->second);
            if (!matRes) {
                 Logger::log(MessageType::SEVERE,"Failed to cast to Material_Resource!");
                continue;
            }
            //calculate Model Matrix in GLM
            glm::mat4 modelGLMmat = extract_Model_Matrix(render);

            //calculate the new mvpGLM matrix
            mvpGLM = projGLMmat * viewGLMmat * modelGLMmat;

            //convert mvpGLM to eigen mvpEigen
            mvpEigen = Converter::convert_from_GLM_to_EigenMatrix(mvpGLM);
            Converter::convert_to_matrix_indices(objRes);
            Converter::convert_to_matrix_vertices(objRes, matRes);
            Converter::convert_to_matrix_colors(objRes, matRes);

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
        else if (light)
        {
            //todo farbe auf glatt weiß setzten und sphere auslagern -> thread oder nur 1 mal initialisieren (1 mal ist lieber)
            glm::mat4 modelGLMmat = extract_Model_Matrix(light);
            mvpGLM = projGLMmat * viewGLMmat * modelGLMmat;
            mvpEigen = Converter::convert_from_GLM_to_EigenMatrix(mvpGLM);
            mShader.setUniform("modelViewProj", mvpEigen);
            nanogui::MatrixXu indices = make_sphere_indices();
            Eigen::MatrixXf vertices = make_sphere_vertices();
            Eigen::MatrixXf color = makeLightColor(light->get_color(),vertices.cols());
            mShader.uploadIndices(indices);
            mShader.uploadAttrib("position", vertices);
            mShader.uploadAttrib("color", color);
            glEnable(GL_DEPTH_TEST);
            mShader.drawIndexed(GL_TRIANGLES, 0, indices.rows() * indices.cols());
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

glm::mat4 Preview_Canvas::extract_Model_Matrix(const shared_ptr<Base_Component>& input) {
    auto result = glm::mat4(1.0f);
    result = glm::translate(result, input->get_position());
    result = glm::rotate(result, glm::radians(input->get_rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    result = glm::rotate(result, glm::radians(input->get_rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    result = glm::rotate(result, glm::radians(input->get_rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    result = glm::scale(result, input->get_scale());
    return result;
}

Eigen::MatrixXf Preview_Canvas::make_sphere_vertices() {
    float radius = 10.0;
    int stacks = 10, slices = 10;

    // Calculate the total number of vertices
    int numVertices = (stacks + 1) * (slices + 1);

    // Initialize the Eigen matrix
    Eigen::MatrixXf result(3, numVertices); // Store vertices in 3 rows: (x, y, z)

    // Generate vertices
    int vertexIndex = 0;
    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI * i / stacks; // Angle from the "north pole"
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * j / slices; // Angle around the equator

            // Calculate vertex position
            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            // Store the vertex in the Eigen matrix (column-major order)
            result(0, vertexIndex) = x; // x-coordinate
            result(1, vertexIndex) = y; // y-coordinate
            result(2, vertexIndex) = z; // z-coordinate

            vertexIndex++;
        }
    }

    return result;
}

nanogui::MatrixXu Preview_Canvas::make_sphere_indices() {
    int stacks = 10, slices = 10;

    // Calculate the total number of triangles
    int numTriangles = stacks * slices * 2;

    // Resize the result matrix to hold all triangle indices
    nanogui::MatrixXu result(3, numTriangles); // 3 rows (triangle vertices) x numTriangles

    // Generate indices for triangles
    int triangleIndex = 0;
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            // First triangle of the quad
            result(0, triangleIndex) = first;
            result(1, triangleIndex) = second;
            result(2, triangleIndex) = first + 1;
            triangleIndex++;

            // Second triangle of the quad
            result(0, triangleIndex) = second;
            result(1, triangleIndex) = second + 1;
            result(2, triangleIndex) = first + 1;
            triangleIndex++;
        }
    }

    return result;
}

Eigen::MatrixXf Preview_Canvas::makeLightColor(glm::vec3 color, int amount) {
    Eigen::MatrixXf result = Eigen::MatrixXf(3,amount);
    for (int col = 0; col < amount; ++col) {
        result(0, col) = color.x;
        result(1, col) = color.y;
        result(2, col) = color.z;
    }
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
            Logger::log(MessageType::INFO, "Shortcut: Light (Ctrl+L)");
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
            Logger::log(MessageType::INFO, "Shortcut: Quit (Ctrl+R)");
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

    // Prüfen, ob das hinzuzufügende Objekt eine Kamera ist
    if (auto cameraComp = std::dynamic_pointer_cast<Camera_Component>(component)) {
        // Falls das Objekt eine Kamera ist, überspringen wir das Hinzufügen
        Logger::log(MessageType::INFO, "Kamera-Komponente existiert bereits. Keine neue Kamera wird hinzugefügt.");
        components.insert({uuid, component});
        forceUpdate();
        return;
    }

    // Überprüfen, ob bereits eine Kamera in der Szene existiert
    bool cameraExists = std::any_of(components.begin(), components.end(), [](const auto& pair) {
        return std::dynamic_pointer_cast<Camera_Component>(pair.second);
    });

    // Falls noch keine Kamera existiert, wird eine Kamera zur Szene hinzugefügt
    if (!cameraExists) {
        // Erzeuge eine neue UUID für die Kamera
        boost::uuids::uuid cameraUUID = boost::uuids::random_generator()();

        // Erstelle die Kamera-Komponente
        auto cameraComp = std::make_shared<Camera_Component>(
            cameraUUID,
            "Camera",
            glm::vec3{0, 65, 100},
            glm::vec3{-35, 0, 0},
            glm::vec3{1, 1, 1},
            60,
            1.77f,
            0.1f,
            1000
        );

        // Kamera zur Szene hinzufügen
        components.insert({cameraUUID, cameraComp});
        Logger::log(MessageType::INFO, "Eine neue Kamera wurde automatisch zur Szene hinzugefügt.");
    }

    // Füge das gegebene Objekt (falls keine Kamera) zur Szene hinzu
    components.insert({uuid, component});
    forceUpdate();
}

void Main_Scene::removeComponent(const boost::uuids::uuid& uuid)
{
    auto render_obj = resources.find(uuid);

    auto render =  dynamic_pointer_cast<Render_Component>(render_obj->second);

    if(render)
    {
        resources.erase(render->get_material_UUID());
        resources.erase(render->get_object_UUID());
    }
    components.erase(uuid);
}


void Main_Scene::addResource(boost::uuids::uuid uuid, const std::shared_ptr<Base_Resource>& object_resource)
{
    resources.insert({uuid, object_resource});
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
