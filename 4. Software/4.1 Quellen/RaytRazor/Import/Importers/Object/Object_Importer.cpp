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

/*bool loadOBJ(
    const char* path,
    nanogui::MatrixXf& out_vertices,    // Verwende Eigen::MatrixXf für Vertices
    nanogui::MatrixXu& out_indices,     // Verwende Eigen::MatrixXu für Indices
    nanogui::MatrixXf& out_uvs,         // Verwende Eigen::MatrixXf für UVs
    nanogui::MatrixXf& out_normals,     // Verwende Eigen::MatrixXf für Normals
    nanogui::MatrixXf& out_colors       // Verwende Eigen::MatrixXf für Colors
) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<Eigen::Vector3f> temp_vertices;
    std::vector<Eigen::Vector2f> temp_uvs;
    std::vector<Eigen::Vector3f> temp_normals;
    std::vector<Eigen::Vector3f> temp_colors;  // Temporäres Array für Farben

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

        if (header == "v") {  // Vertex Position
            Eigen::Vector3f vertex;
            s >> vertex.x() >> vertex.y() >> vertex.z();
            temp_vertices.push_back(vertex);
        } else if (header == "vt") {  // UV
            Eigen::Vector2f uv;
            s >> uv.x() >> uv.y();
            temp_uvs.push_back(uv);
        } else if (header == "vn") {  // Normal
            Eigen::Vector3f normal;
            s >> normal.x() >> normal.y() >> normal.z();
            temp_normals.push_back(normal);
        } else if (header == "vc") {  // Vertex Color (neu hinzugefügt)
            Eigen::Vector3f color;
            s >> color.x() >> color.y() >> color.z();
            temp_colors.push_back(color);
        } else if (header == "f") {  // Face
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            for (int i = 0; i < 3; i++) {
                char slash1, slash2;
                s >> vertexIndex[i] >> slash1 >> uvIndex[i] >> slash2 >> normalIndex[i];
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    // Überprüfen, ob es Farben gibt und die Anzahl der Farben korrekt ist
    if (temp_colors.size() == 0) {
        // Falls keine Farben definiert sind, setze eine Standardfarbe (z.B. weiß)
        Eigen::Vector3f defaultColor(1, 0, 0);
        for (size_t i = 0; i < temp_vertices.size(); i++) {
            temp_colors.push_back(defaultColor);
        }
    }

    // Verarbeite die Indizes und Daten
    out_vertices.resize(3, vertexIndices.size() / 3);
    out_uvs.resize(2, uvIndices.size() / 3);
    out_normals.resize(3, normalIndices.size() / 3);
    out_colors.resize(3, vertexIndices.size() / 3);

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Vertices
        out_vertices.col(i) = temp_vertices[vertexIndex - 1];

        // UVs
        out_uvs.col(i) = temp_uvs[uvIndex - 1];

        // Normals
        out_normals.col(i) = temp_normals[normalIndex - 1];

        // Farben
        out_colors.col(i) = temp_colors[vertexIndex - 1];  // Weisen die Farbe dem Vertex zu

        // Indices (MatrixXu ist die Eigen-Implementierung für unsigned int Matrizen)
        out_indices(i) = i;  // Füge den Index in das Indices-Array ein
    }

    return true;
}
*/