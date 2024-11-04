// SceneParser.cpp
#include "SceneParser.h"
#include <nlohmann/json.hpp> // JSON-Bibliothek
#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp> // JSON-Bibliothek für die Datenverarbeitung

using json = nlohmann::json;

Scene parseScene(const std::string& jsonData) {
    Scene scene; // Das Scene-Objekt, das zurückgegeben wird
    auto data = json::parse(jsonData); // JSON-String in ein JSON-Objekt parsen

    // Entitäten parsen
    for (const auto& [name, entityData] : data["entities"].items()) {
        Entity entity; // Temporäres Entity-Objekt
        entity.uuid = entityData.value("uuid", 0); // UUID der Entität extrahieren

        // Translation-Daten parsen (Position, Rotation, Scale)
        if (entityData.contains("Translation")) {
            const auto& translationData = entityData["Translation"];

            // Position parsen
            if (translationData.contains("position")) {
                const auto& positionData = translationData["position"];
                entity.translation.position = Position{
                    positionData.value("x", 0.0f), // x-Position
                    positionData.value("y", 0.0f), // y-Position
                    positionData.value("z", 0.0f)  // z-Position
                };
            }

            // Rotation parsen
            if (translationData.contains("rotation")) {
                const auto& rotationData = translationData["rotation"];
                entity.translation.rotation = Rotation{
                    rotationData.value("x", 0.0f), // x-Rotation
                    rotationData.value("y", 0.0f), // y-Rotation
                    rotationData.value("z", 0.0f)  // z-Rotation
                };
            }

            // Scale parsen
            if (translationData.contains("scale")) {
                const auto& scaleData = translationData["scale"];
                entity.translation.scale = Scale{
                    scaleData.value("x", 1.0f), // x-Skalierung
                    scaleData.value("y", 1.0f), // y-Skalierung
                    scaleData.value("z", 1.0f)  // z-Skalierung
                };
            }
        }

        // Komponenten parsen (RenderComponent und LightComponent)
        if (entityData.contains("components")) {
            const auto& componentsData = entityData["components"];

            // Render-Komponente parsen
            if (componentsData.contains("RenderComponent")) {
                const auto& renderData = componentsData["RenderComponent"];
                entity.components.renderComponent = RenderComponent{
                    renderData.value("objUUID", 0), // Objekt-UUID
                    renderData.value("matUUID", 0)  // Material-UUID
                };
            }

            // Licht-Komponente parsen
            if (componentsData.contains("LightComponent")) {
                const auto& lightData = componentsData["LightComponent"];
                entity.components.lightComponent = LightComponent{
                    lightData.value("intensity", 0.0f), // Lichtintensität
                    {
                        lightData["color"].value("r", 0), // R-Farbwert
                        lightData["color"].value("g", 0), // G-Farbwert
                        lightData["color"].value("b", 0)  // B-Farbwert
                    }
                };
            }
        }

        // Entität zur Szene hinzufügen
        scene.entities[name] = entity;
    }

    // Ressourcen parsen (Objekte und Materialien)
    if (data.contains("resources")) {
        const auto& resourcesData = data["resources"];

        // Objekte parsen
        for (const auto& [id, objectData] : resourcesData["objects"].items()) {
            scene.resources.objects[std::stoi(id)] = {
                objectData.value("type", ""), // Typ des Objekts
                objectData.value("path", "")  // Pfad zum Objekt
            };
        }

        // Materialien parsen
        for (const auto& [id, materialData] : resourcesData["materials"].items()) {
            scene.resources.materials[std::stoi(id)] = {
                materialData.value("type", ""), // Typ des Materials
                materialData.value("path", "")  // Pfad zum Material
            };
        }
    }

    return scene; // Geparste Szene zurückgeben
}

// Funktion zum Exportieren der Szene in eine JSON-Datei
bool exportSceneToJson(const Scene& scene, const std::string& filePath) {
    json data; // JSON-Objekt zum Speichern der Szenendaten

    // Entitäten zur JSON hinzufügen
    for (const auto& [name, entity] : scene.entities) {
        json entityData;
        entityData["uuid"] = entity.uuid;

        // Translation-Daten hinzufügen
        json translationData;
        translationData["position"] = {
            {"x", entity.translation.position.x},
            {"y", entity.translation.position.y},
            {"z", entity.translation.position.z}
        };
        translationData["rotation"] = {
            {"x", entity.translation.rotation.x},
            {"y", entity.translation.rotation.y},
            {"z", entity.translation.rotation.z}
        };
        translationData["scale"] = {
            {"x", entity.translation.scale.x},
            {"y", entity.translation.scale.y},
            {"z", entity.translation.scale.z}
        };
        entityData["Translation"] = translationData;

        // Komponenten hinzufügen
        json componentsData;

        if (entity.components.renderComponent) {
            componentsData["RenderComponent"] = {
                {"objUUID", entity.components.renderComponent->objUUID},
                {"matUUID", entity.components.renderComponent->matUUID}
            };
        }
        if (entity.components.lightComponent) {
            componentsData["LightComponent"] = {
                {"intensity", entity.components.lightComponent->intensity},
                {"color", {
                    {"r", entity.components.lightComponent->color.r},
                    {"g", entity.components.lightComponent->color.g},
                    {"b", entity.components.lightComponent->color.b}
                }}
            };
        }
        entityData["components"] = componentsData;

        data["entities"][name] = entityData; // Entität zur JSON hinzufügen
    }

    // Ressourcen zur JSON hinzufügen
    json resourcesData;
    for (const auto& [id, object] : scene.resources.objects) {
        resourcesData["objects"][std::to_string(id)] = {
            {"type", object.type},
            {"path", object.path}
        };
    }
    for (const auto& [id, material] : scene.resources.materials) {
        resourcesData["materials"][std::to_string(id)] = {
            {"type", material.type},
            {"path", material.path}
        };
    }
    data["resources"] = resourcesData; // Ressourcen zur JSON hinzufügen

    // JSON in eine Datei schreiben
    std::ofstream outFile(filePath); // Öffne die Datei
    if (!outFile.is_open()) { // Überprüfen, ob die Datei erfolgreich geöffnet wurde
        std::cerr << "Fehler beim Öffnen der Datei: " << filePath << std::endl;
        return false; // Rückgabe von false bei Fehler
    }

    outFile << data.dump(4); // JSON als formatierter String (mit 4 Leerzeichen) in die Datei schreiben
    outFile.close(); // Schließe die Datei

    return true; // Rückgabe von true bei erfolgreichem Export
}
