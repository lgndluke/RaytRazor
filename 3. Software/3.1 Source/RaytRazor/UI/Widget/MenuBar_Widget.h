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
#include <windows.h>
#include <filesystem>
#include <random_generator.hpp>
#include "../../Parsing/Json_Parser.h"
#include "../Scenes/Main/Main_Scene.h"

using namespace nanogui;
using namespace std;

namespace fs = std::filesystem;

class MenuBar_Widget : public Widget {
public:
    explicit MenuBar_Widget(Widget* parent);

    /**
     * @brief Initialisiert die Menüleiste mit vordefinierten Menüs und Aktionen.
     * @author Christian Kasper
     */
    void initialize();

private:
    /**
     * @brief Fügt ein Hauptmenü mit Dropdown-Optionen zur Menüleiste hinzu.
     * @param title Der Name des Hauptmenüs (z. B. File, Add, Help).
     * @param options Die Optionen, die im Dropdown angezeigt werden.
     * @param callbacks Die Callbacks, die für jede Option ausgeführt werden sollen.
     * @author Christian Kasper
     */
    void addMenu(const string& title, const vector<string>& options, const vector<function<void()>>& callbacks);
    /**
     * @brief Macht ein Layout Update in NanoGui, und überschreibt den Effekt auf die PopUp
     * @param ctx, Layout Kontext
     * @author Christian Kasper
     */
    void performLayout(NVGcontext* ctx);

    /**
    * @brief Öffnet eine FileDialog von Windows
    * @author Christian Kasper
    */
    static string openFileDialog();

    /**
    * @brief Öffnet eine DirectoryDialog von Windows
    * @author Christian Kasper
    */
    static std::string openDirectoryDialog();

    /**
    * @brief Prüft bei einem Pfad ob es sich um einen Ordner handelt
    * @author Christian Kasper
    */
    static bool isDirectory(const string& path);
};

#endif // MENUBAR_WIDGET_H
