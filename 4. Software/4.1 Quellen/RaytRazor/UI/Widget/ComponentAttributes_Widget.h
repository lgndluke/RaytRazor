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

    void showAttributesOfComponent(const Base_Component &component);

    void clear();

private:
    void populate_widget(const Base_Component &component);
    std::vector<Widget *> dynamicWidgets;
    int FONT_SIZE = 20;
};

#endif // COMPONENT_ATTRIBUTES_WIDGET_H
