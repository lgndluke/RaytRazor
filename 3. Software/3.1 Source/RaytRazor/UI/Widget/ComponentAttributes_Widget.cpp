#include "../Scenes/Main/Main_Scene.h"
#include "ComponentAttributes_Widget.h"


Vec3WidgetGroup::Vec3WidgetGroup(Widget* parent, const std::string& labelText) {
    label = new Label(parent, labelText, "sans-bold", FONT_SIZE);
    layout = new Widget(parent);
    auto* hLayout = new BoxLayout(Orientation::Horizontal, Alignment::Fill, 0, 0);
    layout->setLayout(hLayout);

    xBox = new TextBox(layout, "-");
    yBox = new TextBox(layout, "-");
    zBox = new TextBox(layout, "-");

    xBox->setFixedSize(Vector2i(107, 30));
    yBox->setFixedSize(Vector2i(107, 30));
    zBox->setFixedSize(Vector2i(107, 30));

    xBox->setEditable(true);
    yBox->setEditable(true);
    zBox->setEditable(true);
}

void Vec3WidgetGroup::setCallback(std::function<void(glm::vec3)> callback) const
{
    xBox->setCallback([this, callback](const std::string& value) -> bool {
        try {
            glm::vec3 vec(std::stof(xBox->value()), std::stof(yBox->value()), std::stof(zBox->value()));
            callback(vec);
            return true;
        } catch (...) {
            return false;
        }
    });
    yBox->setCallback([this, callback](const std::string& value) -> bool {
        try {
            glm::vec3 vec(std::stof(xBox->value()), std::stof(yBox->value()), std::stof(zBox->value()));
            callback(vec);
            return true;
        } catch (...) {
            return false;
        }
    });
    zBox->setCallback([this, callback](const std::string& value) -> bool {
        try {
            glm::vec3 vec(std::stof(xBox->value()), std::stof(yBox->value()), std::stof(zBox->value()));
            callback(vec);
            return true;
        } catch (...) {
            return false;
        }
    });
}

void Vec3WidgetGroup::update(const glm::vec3& value, bool editable) const
{
    xBox->setValue(std::to_string(value.x));
    yBox->setValue(std::to_string(value.y));
    zBox->setValue(std::to_string(value.z));

    xBox->setEditable(editable);
    yBox->setEditable(editable);
    zBox->setEditable(editable);
}

void Vec3WidgetGroup::update(const string value, bool editable) const
{
    if(value == "-")
    {
        xBox->setValue(value);
        yBox->setValue(value);
        zBox->setValue(value);

        xBox->setEditable(editable);
        yBox->setEditable(editable);
        zBox->setEditable(editable);
    }
}


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
    positionGroup = new Vec3WidgetGroup(this, "Position:");
    dynamicWidgets.push_back(positionGroup->label);
    dynamicWidgets.push_back(positionGroup->layout);

    // Rotation
    rotationGroup = new Vec3WidgetGroup(this, "Rotation:");
    dynamicWidgets.push_back(rotationGroup->label);
    dynamicWidgets.push_back(rotationGroup->layout);

    // Scale
    scaleGroup = new Vec3WidgetGroup(this, "Scale:");
    dynamicWidgets.push_back(scaleGroup->label);
    dynamicWidgets.push_back(scaleGroup->layout);


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

    // Color
    colorGroup = new Vec3WidgetGroup(this, "Color:");
    dynamicWidgets.push_back(colorGroup->label);
    dynamicWidgets.push_back(colorGroup->layout);

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

    for(Widget* widget : dynamicWidgets)
    {
        if(auto txtBox = dynamic_cast<TextBox*>(widget))
        {
            txtBox->setEditable(false);
        }
    }
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
    try {
            auto updateTextBox = [component](TextBox* textBox, const std::string& value, bool editable) {
                if (textBox) {
                    textBox->setValue(value);
                    textBox->setEditable(editable);
                }
            };

            // UUID (read-only)
            auto tBox_UUID = dynamic_cast<TextBox*>(dynamicWidgets[1]);
            updateTextBox(tBox_UUID, boost::uuids::to_string(component->get_uuid()), false);
            tBox_UUID->setCallback([component, this](const std::string&) -> bool {
                // UUID sollte in der Regel nicht geändert werden
                return true;
            });

            // Name
            auto tBox_Name = dynamic_cast<TextBox*>(dynamicWidgets[3]);
            updateTextBox(tBox_Name, component->get_name(), true);
            tBox_Name->setCallback([component, this](const std::string& value) -> bool {
                component->set_name(value);
                Main_Scene::setChangesOnComponent(component);
                return true;
            });

            glm::vec3 position = component->get_position();
            positionGroup->update(position, true);
            positionGroup->setCallback([component](const glm::vec3& value) {
                component->set_position(value);
                Main_Scene::setChangesOnComponent(component);
            });

            glm::vec3 rotation = component->get_rotation();
            rotationGroup->update(rotation, true);
            rotationGroup->setCallback([component](const glm::vec3& value) {
                component->set_rotation(value);
                Main_Scene::setChangesOnComponent(component);
            });

            glm::vec3 scale = component->get_scale();
            scaleGroup->update(scale, true);
            scaleGroup->setCallback([component](const glm::vec3& value) {
                component->set_scale(value);
                Main_Scene::setChangesOnComponent(component);
            });

            // Kamera-spezifische Felder
            if (auto camera = std::dynamic_pointer_cast<Camera_Component>(component)) {
                auto tBox_FOV = dynamic_cast<TextBox*>(dynamicWidgets[11]);
                updateTextBox(tBox_FOV, std::to_string(camera->get_fov()), true);
                tBox_FOV->setCallback([camera, this](const std::string& value) -> bool {
                    try {
                        camera->set_fov(std::stof(value));
                        Main_Scene::setChangesOnComponent(camera);
                        return true;
                    } catch (...) {
                        return false;
                    }
                });

                auto tBox_Aspect = dynamic_cast<TextBox*>(dynamicWidgets[13]);
                updateTextBox(tBox_Aspect, std::to_string(camera->get_aspect_ratio()), true);
                tBox_Aspect->setCallback([camera, this](const std::string& value) -> bool {
                    try {
                        camera->set_aspect_ratio(std::stof(value));
                        Main_Scene::setChangesOnComponent(camera);
                        return true;
                    } catch (...) {
                        return false;
                    }
                });

                auto tBox_NearClip = dynamic_cast<TextBox*>(dynamicWidgets[15]);
                updateTextBox(tBox_NearClip, std::to_string(camera->get_near_clip()), true);
                tBox_NearClip->setCallback([camera, this](const std::string& value) -> bool {
                    try {
                        camera->set_near_clip(std::stof(value));
                        Main_Scene::setChangesOnComponent(camera);
                        return true;
                    } catch (...) {
                        return false;
                    }
                });

                auto tBox_FarClip = dynamic_cast<TextBox*>(dynamicWidgets[17]);
                updateTextBox(tBox_FarClip, std::to_string(camera->get_far_clip()), true);
                tBox_FarClip->setCallback([camera, this](const std::string& value) -> bool {
                    try {
                        camera->set_far_clip(std::stof(value));
                        Main_Scene::setChangesOnComponent(camera);
                        return true;
                    } catch (...) {
                        return false;
                    }
                });
            }
            else
            {
                updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[11]), "-", false);
                updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[13]), "-", false);
                updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[15]), "-", false);
                updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[17]), "-", false);
            }

            // Licht-spezifische Felder
            if (auto light = std::dynamic_pointer_cast<Light_Component>(component)) {
                auto tBox_Intensity = dynamic_cast<TextBox*>(dynamicWidgets[19]);
                updateTextBox(tBox_Intensity, std::to_string(light->get_intensity()), true);
                tBox_Intensity->setCallback([light, this](const std::string& value) -> bool {
                    try {
                        light->set_intensity(std::stof(value));
                        Main_Scene::setChangesOnComponent(light);
                        return true;
                    } catch (...) {
                        return false;
                    }
                });

                glm::vec3 color = light->get_color();
                colorGroup->update(color, true);
                colorGroup->setCallback([light](const glm::vec3& value) {
                    light->set_color(value);
                    Main_Scene::setChangesOnComponent(light);
                });
            }
            else
            {
                updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[19]), "-", false);
                colorGroup->update("-", false);
            }

            // Render-spezifische Felder
            if (auto render = std::dynamic_pointer_cast<Render_Component>(component)) {
                auto tBox_ObjectUUID = dynamic_cast<TextBox*>(dynamicWidgets[23]);
                updateTextBox(tBox_ObjectUUID, boost::uuids::to_string(render->get_object_UUID()), false);

                auto tBox_MaterialUUID = dynamic_cast<TextBox*>(dynamicWidgets[25]);
                updateTextBox(tBox_MaterialUUID, boost::uuids::to_string(render->get_material_UUID()), false);
            }
            else
            {
                updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[23]), "-", false);
                updateTextBox(dynamic_cast<TextBox*>(dynamicWidgets[25]), "-", false);
            }
    }
    catch (const std::exception& e)
    {
        Logger::log(MessageType::SEVERE, e.what());
    }
}






