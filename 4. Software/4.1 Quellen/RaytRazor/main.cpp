#include "UI/Scenes/Main/Main_Scene.h"
#include "Utility/Logger/Logger.h"
#include <nanogui/nanogui.h>
#include <iostream>
#include <map>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Parsing/Json_Parser.h"

using namespace nanogui;

int main(int argc, char* argv[]) {

    try
    {
        init();
        nanogui::ref app = new Main_Scene(1920, 1010, "RaytRazor", true);
        app->drawAll();
        app->setVisible(true);
        mainloop();
        shutdown();
    }
    catch (const std::runtime_error &e)
    {
        Logger::log(MessageType::SEVERE, "main(): Caught fatal error: " + std::string(e.what()));
        return -1;
    }

    return 0;
}
/*
// Verwenden von std::variant, um verschiedene Komponenten-Typen zu speichern
    std::map<boost::uuids::uuid, std::variant<Base_Component, Render_Component, Light_Component, Camera_Component>> components;

    // Generiere UUIDs für die Komponenten
    boost::uuids::uuid uuid1 = boost::uuids::random_generator()();
    boost::uuids::uuid uuid2 = boost::uuids::random_generator()();

    // Erstelle die Komponenten-Objekte
    Render_Component component1(uuid1, "Cube", glm::vec3(-5, 0, 20), glm::vec3(0, 90, 25), glm::vec3(2.5, 1, 1), uuid1, uuid1);
    Light_Component component2(uuid2, "Sun", glm::vec3(0, 50, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    // Füge die Komponenten zur Map hinzu
    components[uuid1] = component1;
    components[uuid2] = component2;

    // Erstelle die Ressourcen-Map
    std::map<boost::uuids::uuid, Base_Resource> resources;

    // Generiere UUIDs für die Ressourcen
    boost::uuids::uuid resourceUuid1 = boost::uuids::random_generator()();
    boost::uuids::uuid resourceUuid2 = boost::uuids::random_generator()();

    // Erstelle die Ressourcen-Objekte
    Base_Resource resource1(resourceUuid1, ResourceType::MATERIAL, "~/assets/wood.mtl");
    Base_Resource resource2(resourceUuid2, ResourceType::OBJECT, "~/assets/cube.obj");

    // Füge die Ressourcen zur Map hinzu
    resources[resourceUuid1] = resource1;
    resources[resourceUuid2] = resource2;

    // JSON-Export
    bool success = Json_Parser::exportToJSON("export.json", components, resources);

    if (success) {
        std::cout << "JSON erfolgreich exportiert nach 'export.json'!" << std::endl;
    } else {
        std::cerr << "Fehler beim Exportieren der JSON." << std::endl;
    }

    return 0;
 */