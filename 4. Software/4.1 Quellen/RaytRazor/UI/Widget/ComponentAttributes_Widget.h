#ifndef COMPONENT_ATTRIBUTES_WIDGET_H
#define COMPONENT_ATTRIBUTES_WIDGET_H

#include <nanogui/nanogui.h>
#include "../../../Components/Base_Component.h"
#include <uuid_io.hpp>

using namespace nanogui;
using namespace std;

class ComponentAttributes_Widget : public Widget {
public:
    explicit ComponentAttributes_Widget(Widget *parent);

    void showAttributesOfComponent(const shared_ptr<Base_Component>& component);

    void updateFromComponent(const std::shared_ptr<Base_Component>& component) const;

    void clear();

private:
    void populate_widget(const shared_ptr<Base_Component>& component);

    std::vector<Widget *> dynamicWidgets;
    int FONT_SIZE = 17;
};

static bool parseVec3(const std::string &input, glm::vec3 &output);

static std::string vec3ToString(const glm::vec3 &vec);

#endif // COMPONENT_ATTRIBUTES_WIDGET_H
