#include <iostream>
#include "Parsing/Json_Parser.h"

int main() {

    map<boost::uuids::uuid, Base_Component> components;
    map<boost::uuids::uuid, Base_Resource> resources;

    const string path = "C:/Users/lukas/Desktop/JsonParser_DummyFile.json";

    Json_Parser::parseJSON(path, components, resources);

    if (components.empty()) {
        Logger::log(MessageType::SEVERE, "Test Failed: Components map is empty.");
    } else {
        Logger::log(MessageType::DEBUG, "Components map is populated.");
    }

    // Check if resources map is not empty
    if (resources.empty()) {
        Logger::log(MessageType::SEVERE, "Test Failed: Resources map is empty.");
    } else {
        Logger::log(MessageType::DEBUG, "Resources map is populated.");
    }

}
