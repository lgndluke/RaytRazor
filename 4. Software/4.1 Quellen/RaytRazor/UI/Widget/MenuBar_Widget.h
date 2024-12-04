#ifndef MENUBAR_WIDGET_H
#define MENUBAR_WIDGET_H

#include <nanogui/widget.h>
#include <vector>
#include <string>
#include <functional>
#include <nanogui/popupbutton.h>
#include <nanogui/button.h>
#include <nanogui/label.h>
#include <iostream>
#include "nanogui/layout.h"

using namespace nanogui;
using namespace std;

class MenuBar_Widget : public Widget {
public:
    explicit MenuBar_Widget(Widget* parent);

    /**
     * @brief Initialisiert die Menüleiste mit vordefinierten Menüs und Aktionen.
     */
    void initialize();

private:
    /**
     * @brief Fügt ein Hauptmenü mit Dropdown-Optionen zur Menüleiste hinzu.
     * @param title Der Name des Hauptmenüs (z. B. File, Add, Help).
     * @param options Die Optionen, die im Dropdown angezeigt werden.
     * @param callbacks Die Callbacks, die für jede Option ausgeführt werden sollen.
     */
    void addMenu(const string& title, const vector<string>& options, const vector<function<void()>>& callbacks);
};

#endif // MENUBAR_WIDGET_H
