#include "Scene_Manager.h"

//TODO's:
// -> Scene_Manager::update_canvas() implementieren.
// -> Scene_Manager::update_component() implementieren.

void Scene_Manager::update_canvas(Main_Scene main_Scene)
{
    // Think about structure.
}

void Scene_Manager::update_component(const int id)
{

    // 1. Get Component that is affected.
    Base_Component component = this->components[id];

    // 2. Find out what kind of component it is.

        // 3. Change values depending on component type.

}
