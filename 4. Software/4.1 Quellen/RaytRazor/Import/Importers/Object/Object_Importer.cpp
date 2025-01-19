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

std::vector<Indice> Object_Importer::create_Indices(std::vector<Vertex> vertices, const string &path_to_file, std::vector<std::tuple<int, int, int>> tupelArr)
{
    // Index Daten aus .obj Datei auslesen -> Datei ist bereits geprüft und eine valide .obj Datei.
    std::stringstream ss;
    std::ifstream objFile(path_to_file);
    std::string line;
    std::string prefix;
    std::vector<Indice> indices;
    std::string currentMaterial;
    int counter = 0;


    while (std::getline(objFile, line))
    {
        if(line.empty()) continue;
        ss.clear();
        ss.str(line);
        ss >> prefix;
        int v,vn,vt;

        // extract the material and set it for the following faces
        if (prefix == "usemtl")
        {
            std::string currentWord;
            ss >> currentWord;
            currentMaterial = currentWord;
        }
        if (prefix == "f")
        {
            std::vector<int> vertexIndex;
            std::vector<std::tuple<int, int, int>> indicekeys;
            std::string vertexData;
            while (ss >> vertexData) {
                std::istringstream vertexStream(vertexData);
                std::string vStr, vtStr, vnStr;

                std::getline(vertexStream, vStr, '/');
                std::getline(vertexStream, vtStr, '/');
                std::getline(vertexStream, vnStr, '/');

                GLint v_idx = vStr.empty() ? 0 : std::stoi(vStr);
                GLint vt_idx = vtStr.empty() ? 0 : std::stoi(vtStr);
                GLint vn_idx = vnStr.empty() ? 0 : std::stoi(vnStr);

                auto key = std::make_tuple(v_idx, vt_idx, vn_idx);
                indicekeys.push_back(key);
            }
            for (std::tuple<int, int, int> key : indicekeys)
            {
                auto it = std::find(tupelArr.begin(), tupelArr.end(), key);
                int index = (int)std::distance(tupelArr.begin(), it);
                vertexIndex.push_back(index);
            }
            indices.emplace_back(vertexIndex,currentMaterial);
        }
    }
    return indices;
}


// Hash-Funktion für Indizes
struct IndexHash {
    size_t operator()(const std::tuple<GLint, GLint, GLint>& key) const {
        return std::hash<GLint>()(std::get<0>(key)) ^
               std::hash<GLint>()(std::get<1>(key)) ^
               std::hash<GLint>()(std::get<2>(key));
    }
};



std::pair<std::vector<Indice>,std::vector<Vertex>> Object_Importer::fetch_Object(const string &path_to_file) {
        std::vector<glm::fvec3> vertex_pos;       // v
        std::vector<glm::fvec2> vertex_texcoord; // vt
        std::vector<glm::fvec3> vertex_normal;   // vn

        std::vector<GLint> vertex_pos_indices;       // Index für v
        std::vector<GLint> vertex_texcoord_indices; // Index für vt
        std::vector<GLint> vertex_normal_indices;   // Index für vn

        std::vector<Vertex> vertices;
        std::unordered_map<std::tuple<GLint, GLint, GLint>, size_t, IndexHash> unique_vertex_map;
        std::vector<std::tuple<int, int, int>> indiceKeys;

        std::stringstream ss;
        std::ifstream objFile(path_to_file);
        std::string line;
        std::string prefix;

        if (!objFile.is_open()) {
            //std::cerr << "Datei konnte nicht geöffnet werden: " << path_to_file << std::endl;
            std::vector<Indice> indices;
            std::pair<std::vector<Indice>,std::vector<Vertex>> results = std::make_pair(indices,vertices);
            return results;
        }

        while (std::getline(objFile, line)) {
            if (line.empty()) continue;

            ss.clear();
            ss.str(line);
            ss >> prefix;

            if (prefix == "v") {
                glm::vec3 tmp_vec3;
                ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
                vertex_pos.push_back(tmp_vec3);
            } else if (prefix == "vt") {
                glm::vec2 tmp_vec2;
                ss >> tmp_vec2.x >> tmp_vec2.y;
                vertex_texcoord.push_back(tmp_vec2);
            } else if (prefix == "vn") {
                glm::vec3 tmp_vec3;
                ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
                vertex_normal.push_back(tmp_vec3);
            } else if (prefix == "f") {
                std::string vertexData;
                while (ss >> vertexData) {
                    std::istringstream vertexStream(vertexData);
                    std::string v, vt, vn;

                        std::getline(vertexStream, v, '/');
                        std::getline(vertexStream, vt, '/');
                        std::getline(vertexStream, vn, '/');

                    GLint v_idx = v.empty() ? 0 : std::stoi(v);
                    GLint vt_idx = vt.empty() ? 0 : std::stoi(vt);
                    GLint vn_idx = vn.empty() ? 0 : std::stoi(vn);

                    auto key = std::make_tuple(v_idx, vt_idx, vn_idx);

                    // Prüfen, ob die Kombination bereits existiert
                    if (unique_vertex_map.find(key) == unique_vertex_map.end()) {
                        Vertex vertex;

                        // Position
                        if (v_idx > 0 && v_idx <= static_cast<GLint>(vertex_pos.size())) {
                            vertex.position = vertex_pos[v_idx - 1];
                        }

                        // Texturkoordinaten
                        if (vt_idx > 0 && vt_idx <= static_cast<GLint>(vertex_texcoord.size())) {
                            vertex.texCoord = vertex_texcoord[vt_idx - 1];
                        }

                        // Normalen
                        if (vn_idx > 0 && vn_idx <= static_cast<GLint>(vertex_normal.size())) {
                            vertex.normal = vertex_normal[vn_idx - 1];
                        }

                        vertex.color = glm::vec3(1.f, 1.f, 1.f);
                        vertices.push_back(vertex);

                        // Füge die neue Kombination in die Map ein
                        unique_vertex_map[key] = vertices.size() - 1;
                        indiceKeys.push_back(key);
                    }
                }
            }
        }
    std::vector<Indice> indices = create_Indices(vertices, path_to_file, indiceKeys);
    std::pair<std::vector<Indice>,std::vector<Vertex>> results = std::make_pair(indices,vertices);
    return results;
}




bool Object_Importer::validate_extension(const string &path_to_file, const string &suffix)
{
    if (path_to_file.size() < suffix.size())
        return false;

    return path_to_file.substr(path_to_file.size() - suffix.size()) == suffix;
}
