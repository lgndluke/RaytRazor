#include "ComponentAttributes_Widget.h"

#include <utility>

ComponentAttributes_Widget::ComponentAttributes_Widget(Widget *parent)
    : Widget(parent) {
    setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 30));
}

void ComponentAttributes_Widget::showAttributesOfComponent(const shared_ptr<Base_Component>& component) {
    clear();
    populate_widget(component);
}

void ComponentAttributes_Widget::clear() {
    for (auto *widget : dynamicWidgets) {
        removeChild(widget);
    }
    dynamicWidgets.clear();
}

void ComponentAttributes_Widget::populate_widget(const std::shared_ptr<Base_Component>& component) {
    // UUID (read-only)
    auto *uuidLabel = new Label(this, "UUID:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(uuidLabel);
    auto *uuidValue = new TextBox(this, boost::uuids::to_string(component->get_uuid()));
    uuidValue->setEditable(false); // UUID bleibt read-only
    dynamicWidgets.push_back(uuidValue);

    // Name (editable)
    auto *nameLabel = new Label(this, "Name:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(nameLabel);
    auto *nameValue = new TextBox(this, component->get_name());
    nameValue->setCallback([component](const std::string &value) {
        component->set_name(value); // Name im Component-Objekt aktualisieren
        return true; // Eingabe akzeptieren
    });
    dynamicWidgets.push_back(nameValue);

    // Position (editable)
    auto *posLabel = new Label(this, "Position:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(posLabel);
    auto *posValue = new TextBox(this, vec3ToString(component->get_position()));
    posValue->setCallback([component, this](const std::string &value) {
        glm::vec3 pos;
        if (parseVec3(value, pos)) { // parseVec3 ist eine Hilfsfunktion zum Parsen
            component->set_position(pos); // Position aktualisieren
            return true;
        }
        return false; // Eingabe ablehnen, wenn das Format falsch ist
    });
    dynamicWidgets.push_back(posValue);

    // Rotation (editable)
    auto *rotLabel = new Label(this, "Rotation:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(rotLabel);
    auto *rotValue = new TextBox(this, vec3ToString(component->get_rotation()));
    rotValue->setCallback([component, this](const std::string &value) {
        glm::vec3 rot;
        if (parseVec3(value, rot)) {
            component->set_rotation(rot);
            return true;
        }
        return false;
    });
    dynamicWidgets.push_back(rotValue);

    // Scale (editable)
    auto *scaleLabel = new Label(this, "Scale:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(scaleLabel);
    auto *scaleValue = new TextBox(this, vec3ToString(component->get_scale()));
    scaleValue->setCallback([component, this](const std::string &value) {
        glm::vec3 scale;
        if (parseVec3(value, scale)) {
            component->set_scale(scale);
            return true;
        }
        return false;
    });
    dynamicWidgets.push_back(scaleValue);
}

std::string vec3ToString(const glm::vec3 &vec) {
    return "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
}

bool parseVec3(const std::string &input, glm::vec3 &output) {
    float x, y, z;
    if (sscanf(input.c_str(), "(%f, %f, %f)", &x, &y, &z) == 3) {
        output = glm::vec3(x, y, z);
        return true;
    }
    return false;
}


void ComponentAttributes_Widget::updateFromComponent(const std::shared_ptr<Base_Component>& component) const {
    // UUID (read-only)
    dynamic_cast<TextBox*>(dynamicWidgets[1])->setValue(boost::uuids::to_string(component->get_uuid()));

    // Name (editable)
    dynamic_cast<TextBox*>(dynamicWidgets[3])->setValue(component->get_name());

    // Position (editable)
    auto pos = component->get_position();
    dynamic_cast<TextBox*>(dynamicWidgets[5])->setValue("(" + std::to_string(pos.x) + ", " +
                                                        std::to_string(pos.y) + ", " +
                                                        std::to_string(pos.z) + ")");

    // Rotation (editable)
    auto rot = component->get_rotation();
    dynamic_cast<TextBox*>(dynamicWidgets[7])->setValue("(" + std::to_string(rot.x) + ", " +
                                                        std::to_string(rot.y) + ", " +
                                                        std::to_string(rot.z) + ")");

    // Scale (editable)
    auto scale = component->get_scale();
    dynamic_cast<TextBox*>(dynamicWidgets[9])->setValue("(" + std::to_string(scale.x) + ", " +
                                                        std::to_string(scale.y) + ", " +
                                                        std::to_string(scale.z) + ")");
}

