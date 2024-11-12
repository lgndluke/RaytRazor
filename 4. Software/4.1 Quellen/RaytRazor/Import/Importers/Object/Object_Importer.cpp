#include "Object_Importer.h"

//TODO
// - Define further tasks here.
// - task2
// - task3

using namespace std;

// Attributes (Init of global/static Attributes)

// Constructor
Object_Importer::Object_Importer() = default;

// Destructor
Object_Importer::~Object_Importer() = default;

// Private Methods

// Public Methods
vector<Vertex> Object_Importer::loadObject(const string& filePath)
{

    // Vektoren - Dreiecke
    std::vector<glm::fvec3> vertex_position;    //v
    std::vector<glm::fvec2> vertex_texCoord;    //vt
    std::vector<glm::fvec3> vertex_normal;      //vn

    // Vektoren - Faces (f)
    std::vector<GLint> vertex_position_indices; //Index v
    std::vector<GLint> vertex_texCoord_indices; //Index vt
    std::vector<GLint> vertex_normal_indices;   //Index vn

    // Strukturen - MTL
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
    std::ifstream objFile(filePath);
    std::string line;
    std::string prefix;
    glm::vec3 tmp_vec3;
    glm::vec2 tmp_vec2;
    GLint tmp_int = 0;
    // Endprodukt
    std::vector<Vertex> vertices;

    // Falls Datei nicht geöffnet wurde, Error
    if(!objFile.is_open()) {
        Logger::log(MessageType::SEVERE, "Object_Importer::loadObject(): Datei konnte nicht geöffnet werden!");
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
            vertex_position.push_back(tmp_vec3);
        }
        else if (prefix == "vt") { // Texturen
            ss >> tmp_vec2.x >> tmp_vec2.y;
            vertex_texCoord.push_back(tmp_vec2);
        }
        else if (prefix == "vn") { // Normalen für Richtung
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_normal.push_back(tmp_vec3);
        }
        else if (prefix == "f") { // Faces
            int counter = 0;
            while (ss >> tmp_int) {
                // (f) Indizes einlesen
                if (counter == 0) vertex_position_indices.push_back(tmp_int);
                else if (counter == 1) vertex_texCoord_indices.push_back(tmp_int);
                else if (counter == 2) vertex_normal_indices.push_back(tmp_int);
                // '/' und ' ' überspringen
                if(ss.peek() == '/') {
                    counter++;
                    ss.ignore(1, '/');
                }
                else if (ss.peek() == ' ') {
                    counter++;
                    ss.ignore(1, ' ');
                }
                // Wenn v, vt, vn indizes eingelesen wurden, die nächsten einlesen
                if(counter > 2) counter = 0;
            }
            // Jeweils 3 Mal, da diese informationen auf jeden block (3 jeweils) pro face zutreffen
            if (materials.empty()) materials.emplace_back("");
            material_pos_indices.emplace_back(current_material);
            material_pos_indices.emplace_back(current_material);
            material_pos_indices.emplace_back(current_material);

            smoothness.emplace_back(current_smoothness);
            smoothness.emplace_back(current_smoothness);
            smoothness.emplace_back(current_smoothness);

            if (groups.empty()) groups.emplace_back("");
            group_pos_indices.emplace_back(current_group);
            group_pos_indices.emplace_back(current_group);
            group_pos_indices.emplace_back(current_group);

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
            std::cout << "Unbekannter Pr\x84 \bfix: " << prefix << std::endl;
        }
    }
    // Endprodukt zusammensetzen (Mesh)
    vertices.resize(vertex_position_indices.size(), Vertex());

    // Alle indizes nutzen
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].position = vertex_position[vertex_position_indices[i]-1];
        vertices[i].texCoord = vertex_texCoord[vertex_texCoord_indices[i]-1];
        vertices[i].normal = vertex_normal[vertex_normal_indices[i]-1];
        vertices[i].smoothness = smoothness[i];
        vertices[i].materialName = materials[material_pos_indices[i]];
        vertices[i].groupName = groups[group_pos_indices[i]];
        vertices[i].mtlFileName = mtlFile;
        vertices[i].objectName = objectName;
        vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
    }
    // Endprodukt zurück geben
    return vertices;
}