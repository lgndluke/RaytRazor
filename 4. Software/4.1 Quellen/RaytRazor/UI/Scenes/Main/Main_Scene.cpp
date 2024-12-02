#include "Main_Scene.h"

//TODO's:
// -> Preview_Canvas::drawGL() implementieren.
// -> Main_Scene::Main_Scene() implementieren.
// -> Main_Scene::update() implementieren.

Preview_Canvas::Preview_Canvas(Widget* parent) : GLCanvas(parent)
{}

void Preview_Canvas::drawGL()
{

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
    this->components = map<int, Base_Component>();
    this->resources = map<int, Base_Resource>();

    initialize();
}

void *(*thread_code())(void*)
{
    CApp app;
    app.OnExecute();
    return NULL;
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
    preview_window->setSize(Eigen::Vector2i(preview_width, preview_height));

    const auto component_tree = new Window(this, "Component Tree");
    component_tree->setPosition(Eigen::Vector2i(component_tree_position_x, component_tree_position_y));
    component_tree->setSize(Eigen::Vector2i(component_tree_width, component_tree_height));

    const auto component_attributes = new Window(this, "Component Attributes");
    component_attributes->setPosition(Eigen::Vector2i(component_attributes_position_x, component_attributes_position_y));
    component_attributes->setSize(Eigen::Vector2i(component_attributes_width, component_attributes_height));

    const auto preview_canvas = new Preview_Canvas(preview_window);
    preview_window->addChild(preview_canvas);
    preview_canvas->setPosition({ preview_position_x + 10, preview_position_y + 35 });
    preview_canvas->setSize({preview_width - 20, preview_height - 80});

    const auto raytrace_preview_button = new Button(preview_window, "Raytrace Preview");
    preview_window->addChild(raytrace_preview_button);
    raytrace_preview_button->setCallback([this]()
    {
        Logger::log(MessageType::INFO, "Raytrace Preview Button Clicked!");
        // SDL2 ...
        pthread_t SDL_thread;
        pthread_create(&SDL_thread, NULL, thread_code(), NULL);
    });
    raytrace_preview_button->setSize({(preview_width / 2) - 20, 30});
    raytrace_preview_button->setPosition({preview_position_x + raytrace_preview_button->width() + 25, preview_height - 40});

    const auto load_json_button = new Button(preview_window, "Import Scene");
    preview_window->addChild(load_json_button);
    load_json_button->setCallback([this, preview_canvas]()
    {
        Logger::log(MessageType::INFO, "Import Scene Button Clicked!");

        // Json_Parser::parseJSON();
    });
    load_json_button->setSize({(preview_width / 2) - 20, 30});
    load_json_button->setPosition({ preview_position_x + 15, preview_height - 40});

}

void Main_Scene::update()
{
    //
}
