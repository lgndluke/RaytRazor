#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include <nanogui/nanogui.h>

using namespace std;
using namespace nanogui;

/**
 * @brief Die Klasse Main_Scene repräsentiert das Haupt-Fenster der Anwendung.
 * @author Lukas Jeckle
 */
class Main_Scene final : public Screen
{

    public:

        /**
         * @brief Konstruktor zum Erstellen eines Main_Scene Objektes.
         *
         * @param window_width         Die Breite des zu erstellenden Fensters.
         * @param window_height        Die Höhe des zu erstellenden Fensters.
         * @param window_title         Der Titel des zu erstellenden Fensters.
         * @param is_resizeable        Statische Fenstergröße (false), Dynamische Fenstergröße (true)
         */
        Main_Scene(const int window_width,
                   const int window_height,
                   const string& window_title,
                   const bool is_resizeable);

        /**
         * @brief Methode, um die Main_Scene zu aktualisieren.
         */
        void update();

    private:

        //

};

#endif
