#include "Settings_Scene.h"


Settings_Scene::Settings_Scene(const int window_width,
                       const int window_height,
                       const string& window_title,
                       const bool is_resizeable) : Screen(Eigen::Vector2i(window_width, window_height), window_title, is_resizeable)
{
    this->window_width = window_width;
    this->window_height = window_height;
    this->window_title = window_title;
    this->is_resizeable = is_resizeable;

    initialize();
}

void Settings_Scene::initialize()
{
    Window *window = new Fixed_Window(this, "");
    window->setPosition(Vector2i(0, 0));
    window->setLayout(new GroupLayout());
    window->setFixedSize(Vector2i(200, this->window_height));

    Window *window_Context = new Fixed_Window(this, "");
    window_Context->setPosition(Vector2i(200, 0));
    window_Context->setLayout(new GroupLayout());
    window_Context->setFixedSize(Vector2i(this->window_width - window->width(), this->window_height));


    // Menü mit Buttons
    Widget *menu = new Widget(window);
    menu->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10));

    auto *btnGeneral = new Button(menu, "General");
    btnGeneral->setCallback([window_Context, window, this]() {
    // Bestehenden Inhalt von window_Context löschen
    disposeWindow(window_Context);

    // Neues Fenster erstellen
    auto *window_Context = new Fixed_Window(this, "");
    window_Context->setPosition(Vector2i(200, 0));
    window_Context->setLayout(new GroupLayout());
    window_Context->setFixedSize(Vector2i(this->window_width - window->width(), this->window_height));

    // Resolution Header
    new Label(window_Context, "Resolution", "sans-bold");

    // Resolution Slider
    auto *resolutionSlider = new Slider(window_Context);
    resolutionSlider->setValue(0.5f); // Mittlere Position
    resolutionSlider->setFixedWidth(400);

    // Resolution Options (Labels für Slider)
    auto *resolutionRow = new Widget(window_Context);
    resolutionRow->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 10, 50));
    new Label(resolutionRow, "1280x720", "sans");
    new Label(resolutionRow, "1920x1080", "sans");
    new Label(resolutionRow, "2560x1440", "sans");
    new Label(resolutionRow, "3840x2160", "sans");

    // Theme Header
    new Label(window_Context, "Theme", "sans-bold");

    // Theme Buttons
    auto *themeRow = new Widget(window_Context);
    themeRow->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 10, 10));
    Button *btnLight = new Button(themeRow, "Light-Mode");
    btnLight->setFlags(Button::ToggleButton); // Umschaltbar
    btnLight->setPushed(true); // Standardmäßig aktiv
    btnLight->setBackgroundColor(Color(0, 100, 0, 255)); // Grün für aktiviert
    btnLight->setChangeCallback([](bool state) {
        cout << (state ? "Light-Mode Enabled" : "Light-Mode Disabled") << endl;
    });

    Button *btnDark = new Button(themeRow, "Dark-Mode");
    btnDark->setFlags(Button::ToggleButton); // Umschaltbar
    btnDark->setChangeCallback([](bool state) {
        cout << (state ? "Dark-Mode Enabled" : "Dark-Mode Disabled") << endl;
    });

    // Theme-Button-Verhalten: Nur einer aktiv
    btnLight->setChangeCallback([btnDark](bool state) {
        if (state) btnDark->setPushed(false);
    });
    btnDark->setChangeCallback([btnLight](bool state) {
        if (state) btnLight->setPushed(false);
    });

    // Layout aktualisieren
    performLayout();
    });

    auto *btnLayout = new Button(menu, "Layout");
    btnLayout->setCallback([this, window, window_Context]() {
        disposeWindow(window_Context);
        auto *window_Context = new Fixed_Window(this, "");
        window_Context->setPosition(Vector2i(200, 0));
        window_Context->setLayout(new GroupLayout());
        window_Context->setFixedSize(Vector2i(this->window_width - window->width(), this->window_height));

        auto *widget_layout = new Widget(window_Context);
        widget_layout->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10));
        //auto *Label_layout = new Label(window_Context,"Layout");
        auto *btn_saveLayout = new Button(window_Context, "Save Layout");
        auto *btn_loadLayout = new Button(window_Context, "Load Layout");
        performLayout();
    });

    auto *btnKeyBinds = new Button(menu, "KeyBinds");
    btnKeyBinds->setCallback([this, window, window_Context]() {
        disposeWindow(window_Context);
        auto *window_Context = new Fixed_Window(this, "");
        window_Context->setPosition(Vector2i(200, 0));
        window_Context->setLayout(new GroupLayout());
        window_Context->setFixedSize(Vector2i(this->window_width - window->width(), this->window_height));

        auto *widget_KeyBinds = new Widget(window_Context);
        widget_KeyBinds->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10));
        window_Context->setFixedSize(Vector2i(this->window_width - window->width(), this->window_height));

        std::vector<std::string> labels = {"Camera", "Render", "Import", "Export", "Add Entity"};
        std::vector<std::string> keyBinds = {"Numpad 0", "STRG + R", "STRG + I", "STRG + E", "STRG + A + E"};

           for (size_t i = 0; i < labels.size(); ++i) {
               Widget *row = new Widget(widget_KeyBinds);
               row->setLayout(new GroupLayout());

               auto *label = new Label(row, labels[i] + ":", "sans-bold");
               label->setFontSize(22);


               TextBox *textField = new TextBox(row);
               textField->setValue(keyBinds[i]);
               textField->setEditable(true);
               textField->setFixedSize(Vector2i(200, 25));
               textField->setPlaceholder("Keybind");
           }

        performLayout();
    });

    auto *btnOther = new Button(menu, "...");
    btnOther->setCallback([]() { cout << "Other clicked" << endl; });

    // Version unten links
    auto *versionWidget = new Widget(window);
    versionWidget->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 0));
    versionWidget->setFixedHeight(20);
    auto *versionLabel = new Label(versionWidget, "Version: x.y.z", "sans");

    performLayout();
}

