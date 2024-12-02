#include "Main_Scene.h"

//TODO's:
// -> Main_Scene::Main_Scene() implementieren.
// -> Main_Scene::update() implementieren.
// ->

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
    this->components = map<int, Base_Component>();
    this->resources = map<int, Base_Resource>();

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

    const auto preview_window = new Window(this, "3D-Preview");
    preview_window->setPosition(Eigen::Vector2i(preview_position_x, preview_position_y));
    preview_window->setLayout(new GroupLayout());

    const auto component_tree = new Window(this, "Component Tree");
    component_tree->setPosition(Eigen::Vector2i(component_tree_position_x, component_tree_position_y));
    component_tree->setSize(Eigen::Vector2i(component_tree_width, component_tree_height));

    const auto component_attributes = new Window(this, "Component Attributes");
    component_attributes->setPosition(Eigen::Vector2i(component_attributes_position_x, component_attributes_position_y));
    component_attributes->setSize(Eigen::Vector2i(component_attributes_width, component_attributes_height));

    GLCanvas preview_canvas = new GLCanvas(preview_window);
    preview_canvas.setBackgroundColor({100, 100, 100, 255});
    preview_canvas.setSize({600, 300});

    const auto preview_tools = new Widget(preview_window);
    preview_tools->setLayout(new BoxLayout(Orientation::Horizontal,
                                           Alignment::Middle, 0, 5));

    const auto button1 = new Button(preview_tools, "Raytrace Preview");
    button1->setCallback([this]()
    {
        Logger::log(MessageType::INFO, "Button1 Clicked!");
    });

    performLayout();
    preview_window->setSize(Eigen::Vector2i(preview_width, preview_height));
    preview_canvas.setSize({500, 500});

}

void Main_Scene::update()
{
    //
}
