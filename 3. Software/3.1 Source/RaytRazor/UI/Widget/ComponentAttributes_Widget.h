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

struct Vec3WidgetGroup {
    Label* label;
    Widget* layout;  // Layout, um die 3 TextBoxen horizontal zu platzieren
    TextBox* xBox;
    TextBox* yBox;
    TextBox* zBox;
    int FONT_SIZE = 17;

    /**
    * @brief Konstruktor
    * @author Christian Kasper
    */
    Vec3WidgetGroup(Widget* parent, const std::string& labelText);

    /**
    * @brief Aktualisiert mit den richtgien neuen Werten
    * @param value, neuer Wert
    * @param editable, Bearbeitung ja oder nein (true/false)
    * @author Christian Kasper
    */
    void update(const glm::vec3& value, bool editable) const;

    /**
    * @brief updated mit Dummy Werte "-"
    * @param value, dummy string value
    * @param editable, Bearbeitung ja oder nein (true/false)
    * @author Christian Kasper
    */
    void update(string value, bool editable) const;

    /**
    * @brief Definiert die KlickEvents, schreibt die neuen Werte in die ComponentListe rein
    * @author Christian Kasper
    */
    void setCallback(std::function<void(glm::vec3)> callback) const;
};


class ComponentAttributes_Widget : public Widget {
public:
    /**
    * @brief Konstruktor
    * @param parent, Ueber Widget oder Window an dem es angehangen wird
    * @author Christian Kasper
    */
    explicit ComponentAttributes_Widget(Widget *parent);

    /**
    * @brief Ruft clear() und populate_widget auf
    * @author Christian Kasper
    */
    void showAttributesOfComponent();

    /**
    * @brief Update alle TextBoxen mit den vorhanden Werten aus der Komponenten
    * @author Christian Kasper
    */
    void updateFromComponent(const std::shared_ptr<Base_Component>& component) const;

    /**
    * @brief Löscht alle Childs aus der dynamicWidget Klasse raus
    * @author Christian Kasper
    */
    void clear();

private:
    /**
    * @brief Vorbereitet alle Labels und TextBoxen für Base_Komponent und die Unterklassen
    * @author Christian Kasper
    */
    void populate_widget();


    std::vector<Widget *> dynamicWidgets;
    Vec3WidgetGroup* positionGroup{};
    Vec3WidgetGroup* rotationGroup{};
    Vec3WidgetGroup* scaleGroup{};
    Vec3WidgetGroup* colorGroup{};
    int FONT_SIZE = 17;
};

/**
* @brief Verbesserte Ansicht für ein Vec3 in String
* @author Christian Kasper
*/
static bool parseVec3(const std::string &input, glm::vec3 &output);

/**
* @brief Verbesserte Ansicht für ein String in Vec3
* @author Christian Kasper
*/
static std::string vec3ToString(const glm::vec3 &vec);

#endif // COMPONENT_ATTRIBUTES_WIDGET_H
