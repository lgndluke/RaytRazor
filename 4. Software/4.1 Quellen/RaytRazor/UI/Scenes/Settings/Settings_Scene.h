#ifndef SETTINGS_SCENE_H
#define SETTINGS_SCENE_H
#include <string>
#include "../../../Utility/Logger/Logger.h"
#include <nanogui/nanogui.h>
#include "../Main/Main_Scene.h"

/**
 *@author Christian Kasper
 *@brief Einstellung Fenster mit ihren Unterliegenden Kontexte
 */

using namespace nanogui;
using namespace std;


class Settings_Scene final : public nanogui::Screen
{
    public:
    /**Konstruktur der Einstellungs-Szene
     * @param window_width Window-Breite
     * @param window_height Window-Höhe
     * @param window_title Window-Titel
     * @param is_resizeable Vergrößerbar-Toogle
     */
    Settings_Scene(const int window_width,
                   const int window_height,
                   const string& window_title,
                   const bool is_resizeable);



    private:
    int window_width;
    int window_height;
    string window_title;
    bool is_resizeable;

    /**
     * Initialisierung der Scene auf dem Window
     */
    void initialize();


};

#endif
