#ifndef COMPONENT_ATTRIBUTES_WIDGET_H
#define COMPONENT_ATTRIBUTES_WIDGET_H

#include <nanogui/nanogui.h>
#include "../../../Components/Base_Component.h"
#include "../../../Components/Camera/Camera_Component.h"
#include "../../../Components/Entities/Render_Component.h"
#include "../../../Components/Lighting/Light_Component.h"
#include <uuid_io.hpp>

using namespace nanogui;
using namespace std;

class ComponentAttributes_Widget : public Widget {
public:
    explicit ComponentAttributes_Widget(Widget *parent);

    void showAttributesOfComponent();

    void updateFromComponent(const std::shared_ptr<Base_Component>& component) const;

    void updateComponente(std::shared_ptr<Base_Component>& component);

    void clear();

private:
    void populate_widget();


    std::vector<Widget *> dynamicWidgets;
    int FONT_SIZE = 17;
};

static bool parseVec3(const std::string &input, glm::vec3 &output);

static std::string vec3ToString(const glm::vec3 &vec);

#endif // COMPONENT_ATTRIBUTES_WIDGET_H
