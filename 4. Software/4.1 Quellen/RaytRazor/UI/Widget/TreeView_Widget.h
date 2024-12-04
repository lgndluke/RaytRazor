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

using namespace nanogui;
using namespace std;

class TreeView_Widget : public Widget {
public:
    explicit TreeView_Widget(Widget* parent);

    /**
     * @brief Fügt einen Knoten (Node) zur Baumstruktur hinzu.
     * @param nodeName Der Name des Knotens.
     * @param parentName Der Name des Elternknotens, falls dieser vorhanden ist.
     * @param isLastChild Ist das Letzte Kind des Knoten wenn die Aussage war ist.
     */
    void addNode(const std::string& nodeName, const std::string& parentName = "");

    /**
     * @brief Leert die gesamte Baumstruktur.
     */
    void clear();

private:
    Widget* mContainer; ///< Container für die Baumstruktur
    map<std::string, Widget*> mNodeMap; ///< Map für Knoten und deren Widgets
};

#endif // TREEVIEW_WIDGET_H

