#include "Main_Scene.h"

#include "../../../Converter/Converter.h"
#include "../../../Import/Importers/Object/Object_Importer.h"
#include "../../../Import/Importers/Material/Material_Importer.h"

//TODO's:
// -> Preview_Canvas::drawGL() implementieren.
// -> Main_Scene::Main_Scene() implementieren.
// -> Main_Scene::update() implementieren.

std::map<boost::uuids::uuid, std::shared_ptr<Base_Component>> Main_Scene::components;
std::map<boost::uuids::uuid, std::shared_ptr<Base_Resource>> Main_Scene::resources;

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
    Object_Resource OR = Object_Importer::import_Object(uuid, "C:/Users/chris/CLionProjects/RaytRazor/RaytRazor/5. Modelle/5.1 Beispielmodelle/miscellaneous/miscellaneous/teapot/Teapot.obj").value();
    Material_Resource MR = Material_Importer::import_Material(uuid, "C:/Users/chris/CLionProjects/RaytRazor/RaytRazor/5. Modelle/5.1 Beispielmodelle/miscellaneous/miscellaneous/teapot/Teapot.mtl").value();

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

        Converter::convert_to_matrix_colors(MR);
        Converter::convert_to_matrix_indices(OR);
        Converter::convert_to_matrix_vertices(OR);
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

        //shared_ptr<Camera_Component> camera = dynamic_pointer_cast<Camera_Component>(pair.second);

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

glm::vec3 Preview_Canvas::calculateViewDir(glm::vec3 rotation)
{
    float rotX = glm::radians(rotation.x);
    float rotY = glm::radians(rotation.y);

    glm::vec3 result;

    result.x = cos(rotX) * cos(rotY);
    result.y = sin(rotX);
    result.z = cos(rotX) * sin(rotY);

    return glm::normalize(result);
}

std::pair<glm::vec3, glm::vec3> Preview_Canvas::calculateCameraVectors(glm::vec3 position, glm::vec3 rotation)
{
    glm::vec3 viewDirection = calculateViewDir(rotation);
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), viewDirection));
    glm::vec3 upVec = glm::normalize(glm::cross(viewDirection, right));

    std::pair<glm::vec3, glm::vec3> result = {position + viewDirection, upVec};
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

    this->ids = vector<int>();
    this->components = map<boost::uuids::uuid, shared_ptr<Base_Component>>();
    this->resources = map<boost::uuids::uuid, shared_ptr<Base_Resource>>();

    initialize();
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

    const auto attributesWidget = new ComponentAttributes_Widget(component_attributes);

    auto tree_view = new TreeView_Widget(component_tree, attributesWidget);
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
    load_json_button->setCallback([this, tree_view, attributesWidget]
    {
        try
        {
            Logger::log(MessageType::INFO, "Main_Scene::initialize() - Import Scene Button clicked.");

            //Path muss angepasst werden wenn sich wd die Struktur ändert
            //todo über explorer setzen :)
            string path_to_json = "C:/Users/chris/CLionProjects/RaytRazor/RaytRazor/4. Software/4.1 Quellen/RaytRazor/resources/scenes/JsonParser_DummyFile.json";

            //FileSelector file_Selector();
            //string path_to_json = file_Selector().get_input_path();

            components.clear();
            resources.clear();

            Json_Parser::parseJSON(path_to_json, components, resources);

            tree_view->clear();
            tree_view->addNode("3D-Szene");

            int i = -1;
            for (const auto& component : components)
            {
                if(++i == 0) {
                    //attributesWidget->showAttributesOfComponent(component.second);
                }
                string s = component.second->get_name();
                printf("");
                tree_view->addNode(s, "3D-Szene");
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
    // Update Main_Scene?
}
