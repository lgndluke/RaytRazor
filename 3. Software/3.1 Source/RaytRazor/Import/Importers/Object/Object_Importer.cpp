#include "Object_Importer.h"
#include <iostream>

std::optional<Object_Resource> Object_Importer::import_Object(const boost::uuids::uuid& uuid,
                                                              const string& path_to_file)
{

    if(!validate_extension(path_to_file, ".obj"))
    {
        Logger::log(MessageType::SEVERE, "Object_Importer::import_Object(): Type miss-match -> no '.obj' file: " + path_to_file);
        return std::nullopt;
    }

    if (std::ifstream file(path_to_file); !file.is_open())
    {
        Logger::log(MessageType::SEVERE, "Object_Importer::import_Object(): Unable to open object file: " + path_to_file);
        return std::nullopt;
    }

    std::pair<vector<Indice>,vector<Vertex>> result = fetch_Object(path_to_file);
    std::vector<Indice> indices = result.first;
    std::vector<Vertex> vertices = result.second;

    Object_Resource return_Resource(uuid, path_to_file, indices, vertices);

    return return_Resource;

}

// Hash-Funktion für Indizes
struct IndexHash
{
    size_t operator()(const std::tuple<GLint, GLint, GLint>& key) const
    {
        return std::hash<GLint>()(std::get<0>(key)) ^
               std::hash<GLint>()(std::get<1>(key)) ^
               std::hash<GLint>()(std::get<2>(key));
    }
};

std::pair<std::vector<Indice>,std::vector<Vertex>> Object_Importer::fetch_Object(const string &path_to_file)
{

    std::vector<glm::fvec3> vertex_pos;             // v
    std::vector<glm::fvec2> vertex_texcoord;        // vt
    std::vector<glm::fvec3> vertex_normal;          // vn

    std::vector<GLint> vertex_pos_indices;          // Index für v
    std::vector<GLint> vertex_texcoord_indices;     // Index für vt
    std::vector<GLint> vertex_normal_indices;       // Index für vn

    std::string current_material;

    std::vector<Vertex> vertices;
    std::unordered_map<std::tuple<GLint, GLint, GLint>, size_t, IndexHash> unique_vertex_map;

    std::vector<Indice> indices;

    std::stringstream ss;
    std::ifstream object_file(path_to_file);
    std::string line;
    std::string prefix;

    if (!object_file.is_open())
    {
        Logger::log(MessageType::SEVERE, "Datei konnte nicht geöffnet werden: " + path_to_file);
        std::pair<std::vector<Indice>,std::vector<Vertex>> results = std::make_pair(indices,vertices);
        return results;
    }

    while (std::getline(object_file, line))
    {

        if (line.empty()) continue;

        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "usemtl")
        {
            std::string current_word;
            ss >> current_word;
            current_material = current_word;
        }
        else if (prefix == "v")
        {
            glm::vec3 temp;
            ss >> temp.x >> temp.y >> temp.z;
            vertex_pos.push_back(temp);
        }
        else if (prefix == "vt")
        {
            glm::vec2 temp;
            ss >> temp.x >> temp.y;
            vertex_texcoord.push_back(temp);
        }
        else if (prefix == "vn")
        {
            glm::vec3 temp;
            ss >> temp.x >> temp.y >> temp.z;
            vertex_normal.push_back(temp);
        }
        else if (prefix == "f")
        {
            std::vector<int> vertex_idx;
            std::vector<std::tuple<int, int, int>> current_indice_keys;
            std::string vertex_data;

            while (ss >> vertex_data)
            {
                std::istringstream vertex_stream(vertex_data);
                std::string v_str, vt_str, vn_str;

                std::getline(vertex_stream, v_str, '/');
                std::getline(vertex_stream, vt_str, '/');
                std::getline(vertex_stream, vn_str, '/');

                GLint v_idx  = v_str.empty()  ? 0 : std::stoi(v_str);
                GLint vt_idx = vt_str.empty() ? 0 : std::stoi(vt_str);
                GLint vn_idx = vn_str.empty() ? 0 : std::stoi(vn_str);

                auto key = std::make_tuple(v_idx, vt_idx, vn_idx);

                // Prüfen, ob die Kombination bereits existiert
                if (unique_vertex_map.find(key) == unique_vertex_map.end())
                {
                    Vertex vertex;

                    // Position
                    if (v_idx > 0 && v_idx <= vertex_pos.size()) {
                        vertex.position = vertex_pos[v_idx - 1];
                    }

                    // Texturkoordinaten
                    if (vt_idx > 0 && vt_idx <= vertex_texcoord.size()) {
                        vertex.texCoord = vertex_texcoord[vt_idx - 1];
                    }

                    // Normalen
                    if (vn_idx > 0 && vn_idx <= vertex_normal.size()) {
                        vertex.normal = vertex_normal[vn_idx - 1];
                    }

                    //vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
                    vertex.materialName = current_material;
                    vertices.push_back(vertex);

                    // Füge die neue Kombination in die Map ein
                    unique_vertex_map[key] = vertices.size() - 1;
                }
                current_indice_keys.push_back(key);
            }

            for (std::tuple key : current_indice_keys)
            {
                int index = static_cast<int>(unique_vertex_map.find(key)->second);
                vertex_idx.push_back(index);
            }

            indices.emplace_back(vertex_idx, current_material);

        }
    }

    std::pair<std::vector<Indice>,std::vector<Vertex>> results = std::make_pair(indices,vertices);

    return results;

}

bool Object_Importer::validate_extension(const string &path_to_file, const string &suffix)
{
    if (path_to_file.size() < suffix.size())
        return false;

    return path_to_file.substr(path_to_file.size() - suffix.size()) == suffix;
}
