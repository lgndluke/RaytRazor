#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "../../Components/Base_Component.h"
#include "../../Import/Resources/Base_Resource.h"
#include "../Scenes/Main/Main_Scene.h"
#include <nanogui/nanogui.h>

using namespace std;
using namespace nanogui;

/**
 * @brief Die Klasse Scene_Manager verwaltet die Ressourcen der Szenen.
 * @author Lukas Jeckle
 */
class Scene_Manager
{

    public:

        /**
         * @brief Statische Klasse ohne Konstruktor.
         */
        Scene_Manager() = delete;

        /**
         * @brief Methode, um den Inhalt der @link Main_Scene zu aktualisieren.
         * @param main_Scene                Die zu aktualisierende Main_Scene.
         */
        void update_canvas(Main_Scene main_Scene);

        /**
         * @brief Methode, um die Werte eines @link Base_Component Objekts zu aktualisieren.
         * @param id                        Die ID des zu aktualisierenden Base_Components.
         */
        void update_component(const int id);

        /**
         *
         *
         */
        //void update_resource();

    private:

        vector<int> ids;
        map<int, Base_Component> components;
        map<int, Base_Resource> resources;

};

#endif
