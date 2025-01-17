#include "Json_Parser.h"

#include "../Import/Importers/Material/Material_Importer.h"
#include "../Import/Importers/Object/Object_Importer.h"

//TODO:
// - Methode: Json_Parser::parseJSON()
// -> Ressourcen-Abschnitts -> Importer Aufrufe hinzufügen.
// -> Was passiert mit der Metadata?
// - Implementierung der Methode: Json_Parser::exportToJSON()

using namespace std;
using json = nlohmann::json;

void Json_Parser::parseJSON(const string& path_To_Json,
                            map<boost::uuids::uuid, shared_ptr<Base_Component>>& components,
                            map<boost::uuids::uuid, shared_ptr<Base_Resource>>& resources)
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

        auto render_component = std::make_shared<Render_Component>(uuid, name, position, rotation, scale, object_UUID, material_UUID);
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

        auto light_component = std::make_shared<Light_Component>(uuid, name, position, rotation, scale, intensity, color);
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

        auto camera_component = std::make_shared<Camera_Component>(uuid, name, position, rotation, scale, fov, aspect_ratio, near_clip, far_clip);
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
            std::optional<Object_Resource> object_resource = Object_Importer::import_Object(uuid, path);
            if (object_resource.has_value())
                resources.insert({uuid, std::make_shared<Object_Resource>(object_resource.value())});
        }

        else if (type == "mat")
        {
            optional<Material_Resource> material_resource = Material_Importer::import_Material(uuid, path);
            if (material_resource.has_value())
                resources.insert({uuid, std::make_shared<Material_Resource>(material_resource.value())});
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

bool Json_Parser::exportToJSON(const std::string& exportPath,
                             const std::map<boost::uuids::uuid, variant<Base_Component, Render_Component, Light_Component, Camera_Component>> components,
                             const std::map<boost::uuids::uuid, Base_Resource>& resources)
{
    json root;

    // Initialisieren der JSON-Struktur
    root["renderentity"] = json::array();
    root["lightentity"] = json::array();
    root["cameraentity"] = json::array();
    root["resources"] = json::array();

    if (components.empty()) {
        Logger::log(MessageType::SEVERE, "No components found.");
    }

    for (const auto& [uuid, component] : components) {
        json componentData;
        componentData["uuid"] = boost::uuids::to_string(uuid);

        std::visit([&](auto&& comp)
            {
                using T = std::decay_t<decltype(comp)>;

                componentData["name"] = comp.get_name();
                componentData["Translation"] = {
                    {"position", {{"x", comp.get_position().x}, {"y", comp.get_position().y}, {"z", comp.get_position().z}}},
                    {"rotation", {{"x", comp.get_rotation().x}, {"y", comp.get_rotation().y}, {"z", comp.get_rotation().z}}},
                    {"scale", {{"x", comp.get_scale().x}, {"y", comp.get_scale().y}, {"z", comp.get_scale().z}}}
                };

                if constexpr (std::is_same_v<T, Render_Component>) {
                    root["renderentity"].push_back({
                        {"name", comp.get_name()},
                        {"Translation", componentData["Translation"]},
                        {"components", {
                            {"RenderComponent", {
                                {"objUUID", boost::uuids::to_string(comp.get_object_UUID())},
                                {"matUUID", boost::uuids::to_string(comp.get_material_UUID())}
                            }}
                        }}
                    });
                } else if constexpr (std::is_same_v<T, Light_Component>) {
                    root["lightentity"].push_back({
                        {"name", comp.get_name()},
                        {"Translation", componentData["Translation"]},
                        {"components", {
                            {"LightComponent", {
                                {"intensity", comp.get_intensity()},
                                {"color", {
                                    {"r", comp.get_color().r},
                                    {"g", comp.get_color().g},
                                    {"b", comp.get_color().b}
                                }}
                            }}
                        }}
                    });
                } else if constexpr (std::is_same_v<T, Camera_Component>) {
                    root["cameraentity"].push_back({
                        {"name", comp.get_name()},
                        {"Translation", componentData["Translation"]},
                        {"components", {
                            {"CameraComponent", {
                                {"fov", comp.get_fov()},
                                {"aspectRatio", comp.get_aspect_ratio()},
                                {"nearClip", comp.get_near_clip()},
                                {"farClip", comp.get_far_clip()}
                            }}
                        }}
                    });
                }
            }, component);
    }

    if (resources.empty()) {
        Logger::log(MessageType::SEVERE, "No resources found.");
    }

    for (const auto& [uuid, resource] : resources) {
        json resourceData;
        resourceData["uuid"] = boost::uuids::to_string(resource.get_uuid());
        resourceData["type"] = (resource.get_type() == MATERIAL) ? "mat" : "obj";
        resourceData["path"] = resource.get_path();
        root["resources"].push_back(resourceData);
    }

    // Meta-Daten
    root["metadata"] = {
        {"backgroundColor", {0.1, 0.1, 0.1}},
        {"globalIllumination", true},
        {"renderMode", "pathtracing"},
        {"maxDepth", 5},
        {"samplesPerPixel", 100}
    };

    try {
        std::ofstream file(exportPath);
        file << std::setw(4) << root;
        Logger::log(MessageType::INFO, "JSON export successful.");
        return true;
    } catch (const std::exception& e) {
        Logger::log(MessageType::SEVERE, e.what());
        return false;
    }
}


