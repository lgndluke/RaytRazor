#include "RT_Object_Importer.h"

std::vector<RT_Vertex> RT_Object_Importer::fetch_vertices(const std::string& path_to_file)
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
    std::vector<RT_Vertex> vertices;

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
                if (vertexStream.peek() == '/') { // Check for v//vn
                    vertexStream.get(); // Skip the second '/'
                    vt = ""; // Ensure vt is explicitly empty
                    std::getline(vertexStream, vn); // Parse vn
                } else {
                    std::getline(vertexStream, vt, '/'); // Parse vt if it exists
                    std::getline(vertexStream, vn); // Parse vn if it exists
                }

                // Store indices if they exist
                vertex_pos_indices.push_back(std::stoi(v));
                vertex_texcoord_indices.push_back(vt.empty() ? -1 : std::stoi(vt));
                vertex_normal_indices.push_back(vn.empty() ? -1 : std::stoi(vn));
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
    vertices.resize(vertex_pos_indices.size(), RT_Vertex());

    // Alle indizes nutzen
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].position = vertex_pos[vertex_pos_indices[i]-1];
        if (!vertex_texcoord_indices.empty() && vertex_texcoord_indices[i] > 0 &&
        vertex_texcoord_indices[i] - 1 < vertex_texcoord.size()) {
            vertices[i].texCoord = vertex_texcoord[vertex_texcoord_indices[i] - 1];
        } else {
            vertices[i].texCoord = glm::vec2(0.0, 0.0); // Standardwert
        }

        if (!vertex_normal_indices.empty() && vertex_normal_indices[i] > 0 &&
            vertex_normal_indices[i] - 1 < vertex_normal.size()) {
            vertices[i].normal = vertex_normal[vertex_normal_indices[i] - 1];
            } else {
                vertices[i].normal = glm::vec3(0.0, 0.0, 0.0); // Standardwert
            }
        vertices[i].smoothness = smoothness[i];
        if (!materials.empty() && material_pos_indices[i] >= 0 && material_pos_indices[i] < materials.size()) {
            vertices[i].materialName = materials[material_pos_indices[i]];
        } else {
            vertices[i].materialName = "default";
        }
        if (!groups.empty() && group_pos_indices[i] >= 0 && group_pos_indices[i] < groups.size()) {
            vertices[i].groupName = groups[group_pos_indices[i]];
        } else {
            vertices[i].groupName = "default";
        }
        vertices[i].mtlFileName = mtlFile;
        vertices[i].objectName = objectName;
        vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
    }


    return vertices;
}