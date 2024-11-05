// SceneParser.cpp
#include "SceneParser.h"
#include <nlohmann/json.hpp>  // JSON-Bibliothek für die Datenverarbeitung
#include <iostream>
#include <fstream>

#include "Components/Entities/RenderComponent.h" // RenderComponent-Klassenheader
#include "Components/Lighting/LightComponent.h"   // LightComponent-Klassenheader

using json = nlohmann::json;

/**
 * @brief Parse eine Szene aus einem JSON-String in ein Scene-Objekt.
 *
 * Diese Funktion nimmt JSON-Daten entgegen, die die Eigenschaften und Struktur
 * einer 3D-Szene beschreiben, und erstellt daraus ein `Scene`-Objekt, das in
 * der Rendering-Engine verwendet werden kann.
 *
 * @param jsonData Ein JSON-String, der die Szene beschreibt.
 * @return Scene Das `Scene`-Objekt, das die geparsten Daten enthält.
 */
Scene parseScene(const std::string& jsonData) {
    Scene scene;
    auto data = json::parse(jsonData);

    // Entitäten parsen
    for (const auto& [name, entityData] : data["entities"].items()) {
        Entity entity;
        entity.uuid = entityData.value("uuid", 0);

        // Translation-Daten parsen (Position, Rotation, Scale)
        if (entityData.contains("Translation")) {
            const auto& translationData = entityData["Translation"];

            // Position-Daten
            if (translationData.contains("position")) {
                const auto& positionData = translationData["position"];
                entity.translation.position = Position{
                    glm::vec3(
                        positionData.value("x", 0.0f),
                        positionData.value("y", 0.0f),
                        positionData.value("z", 0.0f)
                    )
                };
            }

            // Rotation-Daten
            if (translationData.contains("rotation")) {
                const auto& rotationData = translationData["rotation"];
                entity.translation.rotation = Rotation{
                    glm::vec3(
                        rotationData.value("x", 0.0f),
                        rotationData.value("y", 0.0f),
                        rotationData.value("z", 0.0f)
                    )
                };
            }

            // Scale-Daten
            if (translationData.contains("scale")) {
                const auto& scaleData = translationData["scale"];
                entity.translation.scale = Scale{
                    glm::vec3(
                        scaleData.value("x", 1.0f),
                        scaleData.value("y", 1.0f),
                        scaleData.value("z", 1.0f)
                    )
                };
            }
        }

        // Komponenten parsen (RenderComponent und LightComponent)
        if (entityData.contains("components")) {
            const auto& componentsData = entityData["components"];

            // Render-Komponente
            if (componentsData.contains("RenderComponent")) {
                const auto& renderData = componentsData["RenderComponent"];
                entity.components.renderComponent = RenderComponent{
                    renderData.value("objUUID", 0),
                    renderData.value("matUUID", 0)
                };
            }

            // Licht-Komponente
            if (componentsData.contains("LightComponent")) {
                const auto& lightData = componentsData["LightComponent"];
                entity.components.lightComponent = LightComponent{
                    lightData.value("intensity", 0.0f),
                    glm::vec3(
                        lightData["color"].value("r", 0),
                        lightData["color"].value("g", 0),
                        lightData["color"].value("b", 0)
                    )
                };
            }
        }

        scene.entities[name] = entity;
    }

    // Ressourcen parsen (Objekte und Materialien)
    if (data.contains("resources")) {
        const auto& resourcesData = data["resources"];

        // Objekte
        for (const auto& [id, objectData] : resourcesData["objects"].items()) {
            scene.resources.objects[std::stoi(id)] = ObjectResource{
                objectData.value("type", ""),
                objectData.value("path", "")
            };
        }

        // Materialien
        for (const auto& [id, materialData] : resourcesData["materials"].items()) {
            scene.resources.materials[std::stoi(id)] = MaterialResource{
                materialData.value("type", ""),
                materialData.value("path", "")
            };
        }
    }

    return scene;
}

/**
 * @brief Exportiert die Szene als JSON-Datei.
 *
 * Diese Funktion speichert die Daten einer `Scene`-Instanz in einer JSON-Datei
 * an einem angegebenen Pfad. Der Export erfolgt im JSON-Format, das von anderen
 * Systemen oder Tools weiterverarbeitet werden kann.
 *
 * @param scene Die zu exportierende Szene.
 * @param filePath Der Pfad zur Ausgabedatei.
 * @return bool True bei erfolgreichem Export, false bei Fehlern.
 */
bool exportSceneToJson(const Scene& scene, const std::string& filePath) {
    json data;

    // Entitäten zur JSON hinzufügen
    for (const auto& [name, entity] : scene.entities) {
        json entityData;
        entityData["uuid"] = entity.uuid;

        // Translation-Daten
        json translationData;
        translationData["position"] = {
            {"x", entity.translation.position.pos.x},
            {"y", entity.translation.position.pos.y},
            {"z", entity.translation.position.pos.z}
        };
        translationData["rotation"] = {
            {"x", entity.translation.rotation.rot.x},
            {"y", entity.translation.rotation.rot.y},
            {"z", entity.translation.rotation.rot.z}
        };
        translationData["scale"] = {
            {"x", entity.translation.scale.scale.x},
            {"y", entity.translation.scale.scale.y},
            {"z", entity.translation.scale.scale.z}
        };
        entityData["Translation"] = translationData;

        // Komponenten
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

        data["entities"][name] = entityData;
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
    data["resources"] = resourcesData;

    // JSON-Daten in Datei schreiben
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filePath << std::endl;
        return false;
    }

    outFile << data.dump(4);
    outFile.close();

    return true;
}
