#include "Main_Scene.h"

//TODO's:
// -> Preview_Canvas::drawGL() implementieren.
// -> Main_Scene::Main_Scene() implementieren.
// -> Main_Scene::update() implementieren.

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

        MatrixXu indices(3, 12);
        indices.col( 0) << 0, 1, 3;
        indices.col( 1) << 3, 2, 1;
        indices.col( 2) << 3, 2, 6;
        indices.col( 3) << 6, 7, 3;
        indices.col( 4) << 7, 6, 5;
        indices.col( 5) << 5, 4, 7;
        indices.col( 6) << 4, 5, 1;
        indices.col( 7) << 1, 0, 4;
        indices.col( 8) << 4, 0, 3;
        indices.col( 9) << 3, 7, 4;
        indices.col(10) << 5, 6, 2;
        indices.col(11) << 2, 1, 5;

        MatrixXf positions(3, 8);
        positions.col(0) << -1,  1,  1;
        positions.col(1) << -1,  1, -1;
        positions.col(2) <<  1,  1, -1;
        positions.col(3) <<  1,  1,  1;
        positions.col(4) << -1, -1,  1;
        positions.col(5) << -1, -1, -1;
        positions.col(6) <<  1, -1, -1;
        positions.col(7) <<  1, -1,  1;

        MatrixXf colors(3, 8);
        colors.col(0) << 1, 0, 0;
        colors.col(1) << 0, 1, 0;
        colors.col(2) << 1, 1, 0;
        colors.col(3) << 0, 0, 1;
        colors.col(4) << 1, 0, 1;
        colors.col(5) << 0, 1, 1;
        colors.col(6) << 1, 1, 1;
        colors.col(7) << 0.5, 0.5, 0.5;

        // ==================================================================

        // Daten in Shader laden.
        mShader.bind();
        mShader.uploadIndices(indices);
        mShader.uploadAttrib("position", positions);
        mShader.uploadAttrib("color", colors);

    }

    // Shader binden und Szene rendern
    mShader.bind();

    Eigen::Matrix4f mvp;
    mvp.setIdentity();
    float time = (float)glfwGetTime();
    mvp.topLeftCorner<3, 3>() = Eigen::Matrix3f(
        Eigen::AngleAxisf(mRotation[0] * time, Eigen::Vector3f::UnitX()) *
        Eigen::AngleAxisf(mRotation[1] * time, Eigen::Vector3f::UnitY()) *
        Eigen::AngleAxisf(mRotation[2] * time, Eigen::Vector3f::UnitZ())
    ) * 0.25f;

    mShader.setUniform("modelViewProj", mvp);

    glEnable(GL_DEPTH_TEST);
    mShader.drawIndexed(GL_TRIANGLES, 0, 12);
    glDisable(GL_DEPTH_TEST);
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
    this->components = map<boost::uuids::uuid, Base_Component>();
    this->resources = map<boost::uuids::uuid, Base_Resource>();

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

    auto tree_view = new TreeView_Widget(component_tree);
    tree_view->setPosition(Eigen::Vector2i(component_tree_position_x, component_tree_position_y + 30));
    tree_view->setSize(Eigen::Vector2i(component_tree_width, component_tree_height - 50));

    const auto component_attributes = new Fixed_Window(this, "Component Attributes");
    component_attributes->setPosition(Eigen::Vector2i(component_attributes_position_x, component_attributes_position_y));
    component_attributes->setSize(Eigen::Vector2i(component_attributes_width, component_attributes_height));

    const auto attributesWidget = new ComponentAttributes_Widget(component_attributes);

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

            string path_to_json = ".\\scenes\\JsonParser_DummyFile.json";

            //FileSelector file_Selector();
            //string path_to_json = file_Selector().get_input_path();

            this->components.clear();
            this->resources.clear();

            Json_Parser::parseJSON(path_to_json, this->components, this->resources);

            tree_view->clear();
            tree_view->addNode("3D-Szene");

            for (const auto& component : this->components)
            {
                tree_view->addNode(component.second.get_name(), "3D-Szene");
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
