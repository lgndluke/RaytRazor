#include "Object_Importer.h"

std::optional<Object_Resource> Object_Importer::import_Object(const boost::uuids::uuid& uuid,
                                                              const string& path_to_file)
{

    if(!validate_extension(path_to_file, ".obj"))
    {
        Logger::log(MessageType::SEVERE, "Object_Importer::import_Object(): Type miss-match: " + path_to_file);
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

bool Object_Importer::validate_extension(const string &path_to_file, const string &suffix)
{
    if (path_to_file.size() < suffix.size())
        return false;

    return path_to_file.substr(path_to_file.size() - suffix.size()) == suffix;
}
