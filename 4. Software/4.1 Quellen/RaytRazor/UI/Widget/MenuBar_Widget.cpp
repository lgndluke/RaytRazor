#include "MenuBar_Widget.h"

//todo bug beim ersten anklicken der Reiter, falsche Pos.

MenuBar_Widget::MenuBar_Widget(Widget* parent) : Widget(parent) {
    setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 10));
    setFixedHeight(30);
}

void MenuBar_Widget::initialize() {
    // File-Menü hinzufügen
    addMenu("File",
            {"Open", "Save", "Exit"},
            {
                []() { std::cout << "Open clicked!" << std::endl; },
                []() { std::cout << "Save clicked!" << std::endl; },
                []() { std::cout << "Exit clicked!" << std::endl; }
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
