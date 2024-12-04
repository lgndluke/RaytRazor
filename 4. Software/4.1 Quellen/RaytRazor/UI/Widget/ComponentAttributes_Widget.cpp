#include "ComponentAttributes_Widget.h"

ComponentAttributes_Widget::ComponentAttributes_Widget(Widget *parent)
    : Widget(parent) {
    setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 40));
}

void ComponentAttributes_Widget::showAttributesOfComponent(const Base_Component &component) {
    clear();
    populate_widget(component);
}

void ComponentAttributes_Widget::clear() {
    for (auto *widget : dynamicWidgets) {
        removeChild(widget);
    }
    dynamicWidgets.clear();
}

void ComponentAttributes_Widget::populate_widget(const Base_Component &component) {
    // UUID
    auto *uuidLabel = new Label(this, "UUID:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(uuidLabel);
    auto *uuidValue = new Label(this, boost::uuids::to_string(component.get_uuid()), "sans", FONT_SIZE);
    dynamicWidgets.push_back(uuidValue);

    // Name
    auto *nameLabel = new Label(this, "Name:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(nameLabel);
    auto *nameValue = new Label(this, component.get_name(), "sans", FONT_SIZE);
    dynamicWidgets.push_back(nameValue);

    // Position
    auto *posLabel = new Label(this, "Position:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(posLabel);
    glm::vec3 pos = component.get_position();
    auto *posValue = new Label(this, "(" + std::to_string(pos.x) + ", " +
        std::to_string(pos.y) + ", " + std::to_string(pos.z) + ")", "sans", FONT_SIZE);
    dynamicWidgets.push_back(posValue);

    // Rotation
    auto *rotLabel = new Label(this, "Rotation:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(rotLabel);
    glm::vec3 rot = component.get_rotation();
    auto *rotValue = new Label(this, "(" + std::to_string(rot.x) + ", " +
        std::to_string(rot.y) + ", " + std::to_string(rot.z) + ")", "sans", FONT_SIZE);
    dynamicWidgets.push_back(rotValue);

    // Scale
    auto *scaleLabel = new Label(this, "Scale:", "sans-bold",FONT_SIZE);
    dynamicWidgets.push_back(scaleLabel);
    glm::vec3 scale = component.get_scale();
    auto *scaleValue = new Label(this, "(" + std::to_string(scale.x) + ", " +
        std::to_string(scale.y) + ", " + std::to_string(scale.z) + ")", "sans",FONT_SIZE);
    dynamicWidgets.push_back(scaleValue);
}
