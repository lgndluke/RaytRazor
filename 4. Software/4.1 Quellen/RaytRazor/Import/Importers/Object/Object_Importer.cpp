#include "Object_Importer.h"

std::optional<Object_Resource> Object_Importer::import_Object(const boost::uuids::uuid& uuid,
                                                              const string& path_to_file)
{

    if(!hasExtension(path_to_file))
    {
        return std::nullopt;
    }

    if (std::ifstream file(path_to_file); !file.is_open())
    {
        Logger::log(MessageType::SEVERE, "Object_Importer::import_Object(): Unable to open object file: " + path_to_file);
        return std::nullopt;
    }

    const vector<int> indices = fetch_indices(path_to_file);
    const vector<Vertex> vertices = fetch_vertices(path_to_file);

    Object_Resource return_Resource(uuid, path_to_file, indices, vertices);
    return return_Resource;

}

std::vector<int> Object_Importer::fetch_indices(const string& path_to_file)
{
    // Index Daten aus .obj Datei auslesen -> Datei ist bereits geprüft und eine valide .obj Datei.
    std::stringstream ss;
    std::ifstream objFile(path_to_file);
    std::string line;
    std::string prefix;
    std::vector<int> indices;

    while (std::getline(objFile, line)) {
        if(line.empty()) continue;
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "f") {
            std::string vertexData;

            while (ss >> vertexData) {
                std::istringstream vertexStream(vertexData);
                std::string v;
                std::getline(vertexStream, v, '/');

                if (!v.empty()) {
                    indices.push_back(std::stoi(v));
                }
            }
        }
    }

    return indices;
}

std::vector<Vertex> Object_Importer::fetch_vertices(const string& path_to_file)
{
    std::vector<glm::fvec3> vertex_pos; //v
    std::vector<glm::fvec2> vertex_texcoord; //vt
    std::vector<glm::fvec3> vertex_normal; //vn
    // Vektoren für Faces (f)
    std::vector<GLint> vertex_pos_indices; //index für v
    std::vector<GLint> vertex_texcoord_indices; //index für vt
    std::vector<GLint> vertex_normal_indices; //index für vn
    // Strukturen für MTL
    std::vector<std::string> materials;
    std::vector<GLint> material_pos_indices;
    GLint current_material = 0;

    std::string mtlFile;
    std::string objectName;

    std::vector<GLint> smoothness;
    GLint current_smoothness = 0;

    std::vector<std::string> groups;
    std::vector<GLint> group_pos_indices;
    GLint current_group = 0;

    // Strukturen zum Dateilesen
    std::stringstream ss;
    std::ifstream objFile(path_to_file);
    std::string line;
    std::string prefix;
    glm::vec3 tmp_vec3;
    glm::vec2 tmp_vec2;
    GLint tmp_int = 0;
    // Endprodukt
    std::vector<Vertex> vertices;

    // Falls Datei nicht geöffnet wurde, Error
    if(!objFile.is_open()) {
        //std::cout<<"Datei konnte nicht ge\x94 \bffnet werden.";
        return vertices;
    }

    //Auslesen von Informationen Zeile für Zeile
    while (std::getline(objFile, line)) {
        //Prefix holen
        if(line.empty()) continue;
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v") { // Punktkoordinaten
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_pos.push_back(tmp_vec3);
        }
        else if (prefix == "vt") { // Texturen
            ss >> tmp_vec2.x >> tmp_vec2.y;
            vertex_texcoord.push_back(tmp_vec2);
        }
        else if (prefix == "vn") { // Normalen für Richtung
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_normal.push_back(tmp_vec3);
        }
        else if (prefix == "f") { // Faces
            std::string vertexData;
            while (ss >> vertexData) {
                std::istringstream vertexStream(vertexData);
                std::string v, vt, vn;

                // Parse v/vt/vn
                std::getline(vertexStream, v, '/');
                std::getline(vertexStream, vt, '/');
                std::getline(vertexStream, vn, '/');

                // Store indices if they exist
                if (!v.empty()) vertex_pos_indices.push_back(std::stoi(v));
                if (!vt.empty()) vertex_texcoord_indices.push_back(std::stoi(vt));
                if (!vn.empty()) vertex_normal_indices.push_back(std::stoi(vn));
            }

            // Ergänzen von Material, Smoothness und Group-Daten
            for (int i = 0; i < 3; i++) { // Dreiecks-Faces (3 Indizes)
                material_pos_indices.push_back(current_material);
                smoothness.push_back(current_smoothness);
                group_pos_indices.push_back(current_group);
            }
        }
        else if (prefix == "s") { // Kantenglättung
            if(size_t pos = ss.str().rfind(' '); ss.str().substr(pos + 1) == "off") current_smoothness = 0;
            ss >> current_smoothness;
        }
        else if (prefix == "mtllib") { // .MTL Einbindung
            size_t pos = ss.str().rfind(' ');
            mtlFile = ss.str().substr(pos + 1);
        }
        else if (prefix == "usemtl") { // Nutzen eines MTL Objektes
            size_t pos = ss.str().rfind(' ');
            if(!materials.empty()) if(materials.back() != ss.str().substr(pos + 1)) current_material++;
            materials.push_back(ss.str().substr(pos + 1));
        }
        else if (prefix == "#") {} //Kommentar
        else if (prefix == "o") { // Objektname
            size_t pos = ss.str().rfind(' ');
            objectName = ss.str().substr(pos + 1);
        }
        else if (prefix == "g") { // Gruppenname
            size_t pos = ss.str().rfind(' ');
            if(!groups.empty()) if(groups.back() != ss.str().substr(pos + 1)) current_group++;
            groups.push_back(ss.str().substr(pos + 1));
        }
        else {
            //std::cout << "Unbekannter Pr\x84 \bfix: " << prefix << std::endl;
        }
    }
    // Endprodukt zusammensetzen (Mesh)
    vertices.resize(vertex_pos_indices.size(), Vertex());

    // Alle indizes nutzen
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].position = vertex_pos[vertex_pos_indices[i]-1];
        vertices[i].texCoord = vertex_texcoord[vertex_texcoord_indices[i]-1];
        vertices[i].normal = vertex_normal[vertex_normal_indices[i]-1];
        vertices[i].smoothness = smoothness[i];
        vertices[i].materialName = materials[material_pos_indices[i]];
        vertices[i].groupName = groups[group_pos_indices[i]];
        vertices[i].mtlFileName = mtlFile;
        vertices[i].objectName = objectName;
        vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
    }


    return vertices;
}

bool Object_Importer::hasExtension(const std::string& fileName) {
    std::vector<std::string> validExtensions = {"obj"};
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos == std::string::npos) {
        return false;
    }

    std::string fileExtension = fileName.substr(dotPos + 1);

    for (const auto& ext : validExtensions) {
        if (fileExtension == ext) {
            return true;
        }
    }
    return false;
}

/*
Impoorter. cpp
#include "Object_Importer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

bool ObjectImporter::loadOBJ(const std::string& path) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<Eigen::Vector3f> temp_vertices;
    std::vector<Eigen::Vector2f> temp_uvs;
    std::vector<Eigen::Vector3f> temp_normals;
    std::vector<Eigen::Vector3f> temp_colors;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string header;
        s >> header;

        // Ignore comments (lines starting with #)
        if (header.empty() || header[0] == '#') {
            continue;
        }


        if (header == "v") {
            Eigen::Vector3f vertex;
            s >> vertex.x() >> vertex.y() >> vertex.z();
            temp_vertices.push_back(vertex);
        } else if (header == "vt") {
            Eigen::Vector2f uv;
            s >> uv.x() >> uv.y();
            temp_uvs.push_back(uv);
        } else if (header == "vn") {
            Eigen::Vector3f normal;
            s >> normal.x() >> normal.y() >> normal.z();
            temp_normals.push_back(normal);
        } else if (header == "vc") {
            Eigen::Vector3f color;
            s >> color.x() >> color.y() >> color.z();
            temp_colors.push_back(color);
        } else if (header == "f") {
            std::vector<unsigned int> vertexIndex, uvIndex, normalIndex;
            std::string vertexData;

            while (s >> vertexData) {
                std::istringstream vertexStream(vertexData);
                std::string v, vt, vn;
                std::getline(vertexStream, v, '/');
                std::getline(vertexStream, vt, '/');
                std::getline(vertexStream, vn, '/');

                if (!v.empty()) vertexIndex.push_back(std::stoi(v));
                if (!vt.empty()) uvIndex.push_back(std::stoi(vt));
                if (!vn.empty()) normalIndex.push_back(std::stoi(vn));
            }

            for (size_t i = 0; i < vertexIndex.size(); i++) {
                if (vertexIndex[i] > temp_vertices.size()) {
                    std::cerr << "Invalid vertex index: " << vertexIndex[i] << std::endl;
                    return false;
                }

                vertexIndices.push_back(vertexIndex[i]);
                if (!uvIndex.empty()) uvIndices.push_back(uvIndex[i]);
                if (!normalIndex.empty()) normalIndices.push_back(normalIndex[i]);
            }
        }
    }

    file.close();

    // Validierung der Größe
    if (vertexIndices.empty() || temp_vertices.empty()) {
        std::cerr << "Invalid OBJ file: No vertices or faces found." << std::endl;
        return false;
    }

    // Standardfarben hinzufügen, falls keine definiert sind
    if (temp_colors.empty()) {
        temp_colors.resize(temp_vertices.size(), Eigen::Vector3f(1.0f, 1.0f, 1.0f));  // Weiß als Standardfarbe
    }

    // Dimensionieren der Member-Matrizen
    m_vertices.resize(3, vertexIndices.size());
    m_indices.resize(3, vertexIndices.size() / 3);
    if (!temp_uvs.empty()) m_uvs.resize(2, uvIndices.size());
    if (!temp_normals.empty()) m_normals.resize(3, normalIndices.size());
    m_colors.resize(3, vertexIndices.size());

    // Daten zuweisen
    for (size_t i = 0; i < vertexIndices.size(); i++) {
        unsigned int vIdx = vertexIndices[i] - 1;
        if (vIdx >= temp_vertices.size()) {
            std::cerr << "Vertex index out of bounds: " << vIdx << std::endl;
            return false;
        }
        m_vertices.col(i) = temp_vertices[vIdx];

        if (!temp_uvs.empty()) {
            unsigned int uvIdx = uvIndices[i] - 1;
            if (uvIdx >= temp_uvs.size()) {
                std::cerr << "UV index out of bounds: " << uvIdx << std::endl;
                return false;
            }
            m_uvs.col(i) = temp_uvs[uvIdx];
        }

        if (!temp_normals.empty()) {
            unsigned int nIdx = normalIndices[i] - 1;
            if (nIdx >= temp_normals.size()) {
                std::cerr << "Normal index out of bounds: " << nIdx << std::endl;
                return false;
            }
            m_normals.col(i) = temp_normals[nIdx];
        }

        unsigned int cIdx = vertexIndices[i] - 1;
        if (cIdx >= temp_colors.size()) {
            std::cerr << "Color index out of bounds: " << cIdx << std::endl;
            return false;
        }
        m_colors.col(i) = temp_colors[cIdx];
    }

    // Indizes
    for (size_t i = 0; i < vertexIndices.size() / 3; i++) {
        m_indices.col(i) << i * 3, i * 3 + 1, i * 3 + 2;
    }

    return true;
}
*/