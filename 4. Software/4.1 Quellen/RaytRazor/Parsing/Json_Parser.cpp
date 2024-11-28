#include "Json_Parser.h"

//TODO:
// - Methode: Json_Parser::parseJSON()
// -> Implementieren des Ressourcen-Abschnitts.
// -> Was passiert mit der Metadata?
// - Implementierung der Methode: Json_Parser::exportToJSON()

using namespace std;
using json = nlohmann::json;

void Json_Parser::parseJSON(const string& path_To_Json,
                            map<boost::uuids::uuid, Base_Component>& components,
                            map<boost::uuids::uuid, Base_Resource>& resources)
{
    std::ifstream file(path_To_Json);

    if (!file.is_open())
    {
        Logger::log(MessageType::SEVERE, "Json_Parser::parseJSON(): Unable to open JSON file: " + path_To_Json);
    }

    json json_data;
    file >> json_data;
    file.close();

    boost::uuids::string_generator uuid_string_generator;

    for (const auto& [key, entity_data] : json_data["renderentity"].items())
    {

        // Variables of type -> Base_Component.
        string uuidStr = entity_data["uuid"].dump();
        uuidStr = uuidStr.substr(1, uuidStr.length()-2);
        boost::uuids::uuid uuid = uuid_string_generator(uuidStr);

        string name = entity_data.value("name", "");

        glm::vec3 position(0.0f);
        glm::vec3 rotation(0.0f);
        glm::vec3 scale(1.0f);

        const vector<string> dimensional_axes = {"x", "y", "z"};

        for (size_t i=0; i<dimensional_axes.size(); i++)
        {
            if (entity_data["Translation"]["position"].contains(dimensional_axes[i]))
            {
                position[i] = entity_data["Translation"]["position"][dimensional_axes[i]].get<float>();
            }
            if (entity_data["Translation"]["rotation"].contains(dimensional_axes[i]))
            {
                rotation[i] = entity_data["Translation"]["rotation"][dimensional_axes[i]].get<float>();
            }
            if (entity_data["Translation"]["scale"].contains(dimensional_axes[i]))
            {
                scale[i] = entity_data["Translation"]["scale"][dimensional_axes[i]].get<float>();
            }
        }

        // Variables of type -> Render_Component.

        string object_uuid_string   = entity_data["components"]["RenderComponent"]["objUUID"].dump();
        string material_uuid_string = entity_data["components"]["RenderComponent"]["matUUID"].dump();

        object_uuid_string = object_uuid_string.substr(1, object_uuid_string.length()-2);
        material_uuid_string = material_uuid_string.substr(1, material_uuid_string.length()-2);

        boost::uuids::uuid object_UUID   = uuid_string_generator(object_uuid_string);
        boost::uuids::uuid material_UUID = uuid_string_generator(material_uuid_string);

        // Adding to components.

        Render_Component render_component(uuid, name, position, rotation, scale, object_UUID, material_UUID);
        components.insert({uuid, render_component});

    }

    for (const auto& [key, entity_data] : json_data["lightentity"].items())
    {
        // Variables of type -> Base_Component.
        string uuidStr = entity_data["uuid"].dump();
        uuidStr = uuidStr.substr(1, uuidStr.length()-2);
        boost::uuids::uuid uuid = uuid_string_generator(uuidStr);

        string name = entity_data.value("name", "");

        glm::vec3 position(0.0f);
        glm::vec3 rotation(0.0f);
        glm::vec3 scale(1.0f);

        const vector<string> dimensional_axes = {"x", "y", "z"};

        for (size_t i=0; i<dimensional_axes.size(); i++)
        {
            if (entity_data["Translation"]["position"].contains(dimensional_axes[i]))
            {
                position[i] = entity_data["Translation"]["position"][dimensional_axes[i]].get<float>();
            }
            if (entity_data["Translation"]["rotation"].contains(dimensional_axes[i]))
            {
                rotation[i] = entity_data["Translation"]["rotation"][dimensional_axes[i]].get<float>();
            }
            if (entity_data["Translation"]["scale"].contains(dimensional_axes[i]))
            {
                scale[i] = entity_data["Translation"]["scale"][dimensional_axes[i]].get<float>();
            }
        }

        // Variables of type -> Light_Component

        float intensity = entity_data["components"]["LightComponent"].value("intensity", 1.0f);

        glm::vec3 color = glm::vec3(
            entity_data["components"]["LightComponent"]["color"].value("r", 0.0f),
            entity_data["components"]["LightComponent"]["color"].value("g", 0.0f),
            entity_data["components"]["LightComponent"]["color"].value("b", 0.0f)
        );

        // Adding to components.

        Light_Component light_component(uuid, name, position, rotation, scale, intensity, color);
        components.insert({uuid, light_component});

    }

    for (const auto& [key, entity_data] : json_data["cameraentity"].items())
    {
        // Variables of type -> Base_Component.
        string uuidStr = entity_data["uuid"].dump();
        uuidStr = uuidStr.substr(1, uuidStr.length()-2);
        boost::uuids::uuid uuid = uuid_string_generator(uuidStr);

        string name = entity_data.value("name", "");

        glm::vec3 position(0.0f);
        glm::vec3 rotation(0.0f);
        glm::vec3 scale(1.0f);

        const vector<string> dimensional_axes = {"x", "y", "z"};

        for (size_t i=0; i<dimensional_axes.size(); i++)
        {
            if (entity_data["Translation"]["position"].contains(dimensional_axes[i]))
            {
                position[i] = entity_data["Translation"]["position"][dimensional_axes[i]].get<float>();
            }
            if (entity_data["Translation"]["rotation"].contains(dimensional_axes[i]))
            {
                rotation[i] = entity_data["Translation"]["rotation"][dimensional_axes[i]].get<float>();
            }
            if (entity_data["Translation"]["scale"].contains(dimensional_axes[i]))
            {
                scale[i] = entity_data["Translation"]["scale"][dimensional_axes[i]].get<float>();
            }
        }

        // Variables of type -> Camera_Component.

        float fov = entity_data["components"]["CameraComponent"].value("fov", 60.0f);
        float aspect_ratio = entity_data["components"]["CameraComponent"].value("aspectRatio", 16.0f / 9.0f);
        float near_clip = entity_data["components"]["CameraComponent"].value("nearClip", 0.1f);
        float far_clip = entity_data["components"]["CameraComponent"].value("farClip", 1000.0f);

        // Adding to components.
        Camera_Component camera_component(uuid, name, position, rotation, scale, fov, aspect_ratio, near_clip, far_clip);
        components.insert({uuid, camera_component});

    }

    for (const auto& [key, resource_data] : json_data["resources"].items())
    {

        string uuidStr = resource_data["uuid"].dump();
        uuidStr = uuidStr.substr(1, uuidStr.length()-2);
        boost::uuids::uuid uuid = uuid_string_generator(uuidStr);

        string type = resource_data.value("type", "");
        string path = resource_data.value("path", "");

        if (type == "obj")
        {
            /*Object_Resource object_resource(type, path);
            resources[uuid] = object_resource;*/
        }
        else if (type == "mat")
        {
            /*Material_Resource material_resource(type, path);
            resources[uuid] = material_resource;*/
        }
        else
        {
            Logger::log(MessageType::SEVERE, "Json_Parser::parseJSON(): Unknown resource type " + type);
        }

    }

    for (const auto& [key, meta_data]: json_data["metadata"].items())
    {
        // Decide what to do with Metadata.
    }

}

bool Json_Parser::exportToJSON(const string& exportPath)
{
    // Methode implementieren.
}