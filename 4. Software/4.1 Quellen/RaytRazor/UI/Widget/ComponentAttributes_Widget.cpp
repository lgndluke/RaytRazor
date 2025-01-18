#include "../Scenes/Main/Main_Scene.h"
#include "ComponentAttributes_Widget.h"


ComponentAttributes_Widget::ComponentAttributes_Widget(Widget *parent)
    : Widget(parent) {
    setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 30));
}

void ComponentAttributes_Widget::showAttributesOfComponent() {
    clear();
    dynamicWidgets.clear();
    populate_widget();
}

void ComponentAttributes_Widget::clear() {
    for (auto *widget : dynamicWidgets) {
        removeChild(widget);
    }
    dynamicWidgets.clear();
}

void ComponentAttributes_Widget::populate_widget() {
    // UUID (read-only, kein Callback erforderlich)
    auto *uuidLabel = new Label(this, "UUID:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(uuidLabel);
    auto *uuidValue = new TextBox(this, "-");
    uuidValue->setFixedSize(Vector2i(320,30));
    uuidValue->setEditable(false); // UUID ist nicht editierbar
    dynamicWidgets.push_back(uuidValue);

    // Name
    auto *nameLabel = new Label(this, "Name:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(nameLabel);
    auto *nameValue = new TextBox(this, "-");
    nameValue->setEditable(true);
    dynamicWidgets.push_back(nameValue);

    // Position
    auto *posLabel = new Label(this, "Position:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(posLabel);
    auto *posValue = new TextBox(this, "-");
    dynamicWidgets.push_back(posValue);

    // Rotation
    auto *rotLabel = new Label(this, "Rotation:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(rotLabel);
    auto *rotValue = new TextBox(this, "-");
    rotValue->setEditable(true);
    dynamicWidgets.push_back(rotValue);

    // Scale
    auto *scaleLabel = new Label(this, "Scale:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(scaleLabel);
    auto *scaleValue = new TextBox(this, "-");
    scaleValue->setEditable(true);
    dynamicWidgets.push_back(scaleValue);

    // Kamera-spezifische Felder
    auto *fovLabel = new Label(this, "FOV:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(fovLabel);
    auto *fovValue = new TextBox(this, "-");
    fovValue->setEditable(true);
    dynamicWidgets.push_back(fovValue);

    auto *aspectLabel = new Label(this, "Aspect Ratio:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(aspectLabel);
    auto *aspectValue = new TextBox(this, "-");
    aspectValue->setEditable(true);
    dynamicWidgets.push_back(aspectValue);

    auto *near_clipLabel = new Label(this, "Near clip:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(near_clipLabel);
    auto *near_clipValue = new TextBox(this, "-");
    near_clipValue->setEditable(true);
    dynamicWidgets.push_back(near_clipValue);

    auto *far_clipLabel = new Label(this, "Far Clip:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(far_clipLabel);
    auto *far_clipValue = new TextBox(this, "-");
    far_clipValue->setEditable(true);
    dynamicWidgets.push_back(far_clipValue);

    // Licht-spezifische Felder
    auto *intensityLabel = new Label(this, "Intensity:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(intensityLabel);
    auto *intensityValue = new TextBox(this, "-");
    intensityValue->setEditable(true);
    dynamicWidgets.push_back(intensityValue);

    auto *colorLabel = new Label(this, "Color:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(colorLabel);
    auto *colorValue = new TextBox(this, "-");
    colorValue->setEditable(true);
    dynamicWidgets.push_back(colorValue);

    // Render-spezifische Felder
    auto *objectLabel = new Label(this, "Object UUID:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(objectLabel);
    auto *objectValue = new TextBox(this, "-");
    objectValue->setEditable(false); // UUID bleibt unveränderlich
    dynamicWidgets.push_back(objectValue);

    auto *materialLabel = new Label(this, "Material UUID:", "sans-bold", FONT_SIZE);
    dynamicWidgets.push_back(materialLabel);
    auto *materialValue = new TextBox(this, "-");
    materialValue->setEditable(false); // UUID bleibt unveränderlich
    dynamicWidgets.push_back(materialValue);
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
    // Hilfsfunktion für allgemeine Updates
    auto updateTextBox = [component](TextBox* textBox, const std::string& value, bool editable) {
        if (textBox) {
            textBox->setValue(value);
            textBox->setEditable(editable);
            textBox->setCallback([component, textBox](const string&) -> bool {
                component->set_name(textBox->value());
                Main_Scene::setChangesOnComponent(component);
                return true;
            });
        }
    };

    // UUID
    updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[1]), boost::uuids::to_string(component->get_uuid()), false);

    // Name
    updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[3]), component->get_name(), true);

    // Position
    updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[5]), vec3ToString(component->get_position()), true);

    // Rotation
    updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[7]), vec3ToString(component->get_rotation()), true);

    // Scale
    updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[9]), vec3ToString(component->get_scale()), true);

    // Kamera-spezifische Felder
    if (auto camera = std::dynamic_pointer_cast<Camera_Component>(component)) {
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[11]), std::to_string(camera->get_fov()), true);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[13]), std::to_string(camera->get_aspect_ratio()), true);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[15]), std::to_string(camera->get_near_clip()), true);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[17]), std::to_string(camera->get_far_clip()), true);
    } else {
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[11]), "-", false);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[13]), "-", false);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[15]), "-", false);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[17]), "-", false);
    }

    // Licht-spezifische Felder
    if (auto light = std::dynamic_pointer_cast<Light_Component>(component)) {
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[19]), std::to_string(light->get_intensity()), true);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[21]), vec3ToString(light->get_color()), true);
    } else {
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[19]), "-", false);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[21]), "-", false);
    }

    // Render-spezifische Felder
    if (auto render = std::dynamic_pointer_cast<Render_Component>(component)) {
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[23]), boost::uuids::to_string(render->get_object_UUID()), false);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[25]), boost::uuids::to_string(render->get_material_UUID()), false);
    } else {
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[23]), "-", false);
        updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[25]), "-", false);
    }
}






