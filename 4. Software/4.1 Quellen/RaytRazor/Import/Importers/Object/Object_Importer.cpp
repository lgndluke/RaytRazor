#include "Object_Importer.h"

//TODO's:
// - Object_Importer::import_Object() -> Prüfen, ob Datei im .obj Format vorliegt.
// - Object_Importer::fetch_indices() implementieren.
// - Object_Importer::fetch_vertices() implementieren.

std::optional<Object_Resource> Object_Importer::import_Object(const boost::uuids::uuid& uuid,
                                                              const string& path_to_file)
{

    // Prüfung, ob Datei im .obj Format vorliegt hier.

    if (std::ifstream file(path_to_file); !file.is_open())
    {
        Logger::log(MessageType::SEVERE, "Object_Importer::import_Object(): Unable to open object file: " + path_to_file);
        return std::nullopt;
    }

    const vector<int> indices = fetch_indices(uuid, path_to_file);
    const vector<Vertex> vertices = fetch_vertices(uuid, path_to_file);

    Object_Resource return_Resource(uuid, path_to_file, indices, vertices);
    return return_Resource;

}

std::vector<int> Object_Importer::fetch_indices(const boost::uuids::uuid& uuid,
                                                const string& path_to_file)
{

    // Index Daten aus .obj Datei auslesen -> Datei ist bereits geprüft und eine valide .obj Datei.
    return vector<int>{};

}

std::vector<Vertex> Object_Importer::fetch_vertices(const boost::uuids::uuid& uuid,
                                                    const string& path_to_file)
{

    // Material Daten aus .mtl Datei auslesen -> Datei ist bereits geprüft und eine valide .mtl Datei.
    return vector<Vertex>{};

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