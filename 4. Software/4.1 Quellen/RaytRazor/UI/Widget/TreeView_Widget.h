//
// Created by chris on 04.12.2024.
//

#ifndef TREEVIEW_WIDGET_H
#define TREEVIEW_WIDGET_H

#include <map>
#include <nanogui/widget.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/label.h>
#include <nanogui/button.h>
#include "nanogui/layout.h"
#include <string>
#include <vector>
#include "ComponentAttributes_Widget.h"
#include "../utility/Custom_Label.h"
#include "../utility/CustomVScrollPanel.h"

using namespace std;

class TreeView_Widget : public Widget {
public:

    explicit TreeView_Widget(Widget* parent, ComponentAttributes_Widget* attributes);
    void addParent(const std::string& parentName);

    /**
     * @brief Fügt einen Knoten (Node) zur Baumstruktur hinzu.
     * @param nodeName Der Name des Knotens.
     * @param parentName Der Name des Elternknotens, falls dieser vorhanden ist.
     * @param isLastChild Ist das Letzte Kind des Knoten wenn die Aussage war ist.
     */
    void addNode(const std::shared_ptr<Base_Component>& nodeName, const std::string& parentName = "");
    void cleanUpUnusedNodes();

    /**
     * @brief Leert die gesamte Baumstruktur.
     */
    void clear();

    void update();

private:
    Widget* mContainer; ///< Container für die Baumstruktur
    map<std::string, Widget*> mNodeMap; ///< Map für Knoten und deren Widgets
    ComponentAttributes_Widget *mAttributes;
    Custom_Label* mCurrentSelectedLabel = nullptr;
    std::map<boost::uuids::uuid, std::string> mTrackedObjects;
    VScrollPanel* mScrollPanel;
};

#endif // TREEVIEW_WIDGET_H
