#include "Json_Parser.h"

//TODO:
// - Implementierung der Methode: Json_Parser::parseJSON() const
// - Implementierung der Methode: Json_Parser::exportToJSON() const

using namespace std;
using json = nlohmann::json;

static map<boost::uuids::uuid, Base_Component> Json_Parser::parseJSON(const string& pathToJson) const
{
    // Methode implementieren.
    auto jsonData = json::parse(pathToJson);
    Base_Component Components[jsonData.size()];

    for (const auto& entityData : jsonData["renderentity"].items())
    {
        // Variables of type -> Base_Component.

        string name = entityData.value("name", 0);
        boost::uuids::uuid uuid = entityData.value("uuid", 0);

        glm::vec3 position = glm::vec3(entityData["Translation"]["position"]["x"],
                                       entityData["Translation"]["position"]["y"],
                                       entityData["Translation"]["position"]["z"]);

        glm::vec3 rotation = glm::vec3(entityData["Translation"]["rotation"]["x"],
                                       entityData["Translation"]["rotation"]["y"],
                                       entityData["Translation"]["rotation"]["z"]);

        glm::vec3 scale    = glm::vec3(entityData["Translation"]["scale"]["x"],
                                       entityData["Translation"]["scale"]["y"],
                                       entityData["Translation"]["scale"]["z"]);

        // Variables of type -> Render_Component.

        boost::uuids::uuid objectId   = entityData["components"]["RenderComponent"]["objUUID"];
        boost::uuids::uuid materialId = entityData["components"]["RenderComponent"]["matUUID"];

        // TODO: Create and add Render_Component to array.
    }

    for (const auto& entityData : jsonData["lightentity"].items())
    {
        // Variables of type -> Base_Component.

        string name = entityData.value("name", 0);
        boost::uuids::uuid uuid = entityData.value("uuid", 0);

        glm::vec3 position = glm::vec3(entityData["Translation"]["position"]["x"],
                                       entityData["Translation"]["position"]["y"],
                                       entityData["Translation"]["position"]["z"]);

        // Variables of type -> Light_Component

        float intensity = entityData["Components"]["LightComponent"]["intensity"];
        glm::vec3 color = glm::vec3(entityData["components"]["LightComponent"]["color"]["r"],
                                    entityData["components"]["LightComponent"]["color"]["g"]
                                    entityData["components"]["LightComponent"]["color"]["b"],)

        // TODO: Create and add Light_Component to array.

    }

    for (const auto& entityData : jsonData["cameraentity"].items())
    {
        // Variables of type -> Base_Component.

        string name = entityData.value("name", 0);
        boost::uuids::uuid uuid = entityData.value("uuid", 0);

        glm::vec3 position = glm::vec3(entityData["Translation"]["position"]["x"],
                                       entityData["Translation"]["position"]["y"],
                                       entityData["Translation"]["position"]["z"]);

        glm::vec3 rotation = glm::vec3(entityData["Translation"]["rotation"]["x"],
                                       entityData["Translation"]["rotation"]["y"],
                                       entityData["Translation"]["rotation"]["z"]);

        // Variables of type -> Camera_Component.

        float fov = entityData["components"]["cameraComponent"]["fov"];
        float aspect = entityData["components"]["cameraComponent"]["aspectRatio"];
        float near = entityData["components"]["cameraComponent"]["nearClip"];
        float far = entityData["components"]["cameraComponent"]["farClip"];

        // TODO: Create and add Camera_Component to array.

    }

    for (const auto& resourceData : jsonData["resources"].items())
    {

        boost::uuids::uuid uuid = resourceData.value("uuid", 0);
        string type = resourceData.value("type", 0);
        string path = resourceData.value("path", 0);

        // TODO: Create and add according Material/Object-Resource, depending on type.

    }

    for (const auto& metadataData : jsonData["metadata"].items())
    {
        // Variables of type -> //TODO METADATA?

        glm::vec3 backgroundColor = glm::vec3(metadataData["backgroundColor"][0],
                                              metadataData["backgroundColor"][1],
                                              metadataData["backgroundColor"][2]);

        boolean globalIllumination = metadataData["globalIllumination"];
        string renderMode = metadataData["renderMode"];
        int maxDepth = metadataData["maxDepth"];
        int samplesPerPixel = metadataData["samplesPerPixel"];

        // TODO: Create and add MetaData Object to array?

    }

}

static boolean Json_Parser::exportToJSON(const string& exportPath) const;
{
    // Methode implementieren.
}