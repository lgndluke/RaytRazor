#include "Material_Importer.h"

//TODO's:
// - Material_Importer::import_Material() -> Prüfen, ob Datei im .mtl Format vorliegt.
// - Material_Importer::fetch_indices() implementieren.
// - Material_Importer::fetch_materials() implementieren.

std::optional<Material_Resource> Material_Importer::import_Material(const boost::uuids::uuid& uuid,
                                                                    const string& path_to_file)
{

    // Prüfung, ob Datei im .mtl Format vorliegt hier.

    if (std::ifstream file(path_to_file); !file.is_open())
    {
        Logger::log(MessageType::SEVERE, "Material_Importer::import_Material(): Unable to open material file: " + path_to_file);
        return std::nullopt;
    }

    const vector<int> indices = fetch_indices(uuid, path_to_file);
    const vector<Material> materials = fetch_materials(uuid, path_to_file);

    Material_Resource return_Resource(uuid, path_to_file, indices, materials);
    return return_Resource;

}

std::vector<int> Material_Importer::fetch_indices(const boost::uuids::uuid& uuid,
                                                  const string& path_to_file)
{

    // Index Daten aus .mtl Datei auslesen -> Datei ist bereits geprüft und eine valide .mtl Datei.
    return vector<int>{};

}

std::vector<Material> Material_Importer::fetch_materials(const boost::uuids::uuid& uuid,
                                                         const string& path_to_file)
{

    // Material Daten aus .mtl Datei auslesen -> Datei ist bereits geprüft und eine valide .mtl Datei.
    return vector<Material>{};

}

// Public Methods
/*vector<Material> Material_Importer::loadMaterial(const string& filePath)
{
    // Vektoren - Materialbeschreibung
    std::vector<glm::vec3> vec_ambient;         // Ka
    std::vector<glm::vec3> vec_diffuse;         // Kd
    std::vector<glm::vec3> vec_specular;        // Ks
    std::vector<GLfloat> vec_transparency;      // d
    std::vector<GLfloat> vec_shininess;         // Ns
    std::vector<GLfloat> vec_illumination_mode; // illum
    std::vector<std::string> names;             // newmtl

    // Vektoren - TexturMaps
    std::vector<std::string> map_ambient;       //map_Ka
    std::vector<std::string> map_diffuse;       //map_Kd
    std::vector<std::string> map_specular;      //map_Ks
    std::vector<std::string> map_transparency;  //map_d
    std::vector<std::string> map_bump;          //bump

    std::vector<GLint> indices_map_Ka;
    std::vector<GLint> indices_map_Kd;
    std::vector<GLint> indices_map_Ks;
    std::vector<GLint> indices_map_d;
    std::vector<GLint> indices_map_bump;

    GLint currentMat = -1;
    // Endprodukt
    std::vector<Material> materials;
    // Strukturen zum Dateilesen
    std::stringstream ss;
    std::ifstream file(filePath);
    std::string line;
    std::string prefix;
    glm::vec3 tmp_vec3;
    GLfloat tmp_float = 0;

    // Falls Datei nicht geöffnet wurde, Error
    if(!file.is_open()) {
        Logger::log(MessageType::SEVERE, "Material_Importer::loadMaterial(): Datei konnte nicht geöffnet werden!");
        return materials;
    }

    //Auslesen von Informationen Zeile für Zeile
    while (std::getline(file, line)) {
        //Prefix holen
        if(line.empty()) continue;
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "Ka") { // Umgebungsfarbe
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vec_ambient.push_back(tmp_vec3);
        }
        else if (prefix == "Kd") { // Diffuse Farbe
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vec_diffuse.push_back(tmp_vec3);
        }
        else if (prefix == "Ks") { // Spiegelfarbe
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vec_specular.push_back(tmp_vec3);
        }
        else if (prefix == "d") { // Transparenz
            ss >> tmp_float;
            vec_transparency.push_back(tmp_float);
        }
        else if (prefix == "#") {} // Kommentar
        else if (prefix == "Ns") { // Glanz
            ss >> tmp_float;
            vec_shininess.push_back(tmp_float);
        }
        else if (prefix == "illum") { // Beleuchtungsmodell
            ss >> tmp_float;
            vec_illumination_mode.push_back(tmp_float);
        }
        else if (prefix == "newmtl") { // Materialname
            size_t pos = ss.str().rfind(' ');
            names.push_back(ss.str().substr(pos + 1));
            currentMat++;
        }
        else if (prefix == "map_Ka") { // Umgebungsfarben Textur
            size_t pos = ss.str().rfind(' ');
            map_ambient.push_back(ss.str().substr(pos + 1));
            indices_map_Ka.push_back(currentMat);
        }
        else if (prefix == "map_Kd") { // Diffusfarben Textur
            size_t pos = ss.str().rfind(' ');
            map_diffuse.push_back(ss.str().substr(pos + 1));
            indices_map_Kd.push_back(currentMat);
        }
        else if (prefix == "map_Ks") { // Spiegelfarben Textur
            size_t pos = ss.str().rfind(' ');
            map_specular.push_back(ss.str().substr(pos + 1));
            indices_map_Ks.push_back(currentMat);
        }
        else if (prefix == "map_d") { // Transparenz Textur
            size_t pos = ss.str().rfind(' ');
            map_transparency.push_back(ss.str().substr(pos + 1));
            indices_map_d.push_back(currentMat);
        }
        else if (prefix == "bump") { // Unebenheits Textur
            size_t pos = ss.str().rfind(' ');
            map_bump.push_back(ss.str().substr(pos + 1));
            indices_map_bump.push_back(currentMat);
        }
        else {
            Logger::log(MessageType::SEVERE, "Material_Importer::loadMaterial(): Unbekannter Präfix: " + prefix + "!");
        }
    }
    // Endprodukt zusammensetzen (Mesh)
    //Falls die maps nicht genutzt wurden, -1 rein
    if (map_ambient.empty()) {
        map_ambient.emplace_back("");
        indices_map_Ka.emplace_back(-1);
    }
    if (map_diffuse.empty()) {
        map_diffuse.emplace_back("");
        indices_map_Kd.emplace_back(-1);
    }
    if (map_specular.empty()) {
        map_specular.emplace_back("");
        indices_map_Ks.emplace_back(-1);
    }
    if (map_transparency.empty()) {
        map_transparency.emplace_back("");
        indices_map_d.emplace_back(-1);
    }
    if (map_bump.empty()) {
        map_bump.emplace_back("");
        indices_map_bump.emplace_back(-1);
    }
    materials.resize(names.size(), Material());
    //Temporärer Index für die Zuteilung der Maps an die richtigen Materialien
    std::vector<int> tmp_ind = {0, 0, 0, 0, 0};
    // Alle indizes nutzen
    for (size_t i = 0; i < materials.size(); i++) {
        materials[i].name = names[i];
        materials[i].ambient = vec_ambient[i];
        materials[i].diffuse = vec_diffuse[i];
        materials[i].specular = vec_specular[i];
        materials[i].transparency = vec_transparency[i];
        materials[i].shininess = vec_shininess[i];
        materials[i].illumination = vec_illumination_mode[i];
        if(i == indices_map_Ka[tmp_ind[0]]) {
            materials[i].map_Ka = map_ambient[tmp_ind[0]];
            tmp_ind[0]++;
        }
        else materials[i].map_Ka = "";
        if(i == indices_map_Kd[tmp_ind[1]]) {
            materials[i].map_Kd = map_diffuse[tmp_ind[1]];
            tmp_ind[1]++;
        }
        else materials[i].map_Kd = "";
        if(i == indices_map_Ks[tmp_ind[2]]) {
            materials[i].map_Ks = map_specular[tmp_ind[2]];
            tmp_ind[2]++;
        }
        else materials[i].map_Ks = "";
        if(i == indices_map_d[tmp_ind[3]]) {
            materials[i].map_d = map_transparency[tmp_ind[3]];
            tmp_ind[3]++;
        }
        else materials[i].map_d = "";
        if(i == indices_map_bump[tmp_ind[4]]) {
            materials[i].bump = map_bump[tmp_ind[4]];
            tmp_ind[4]++;
        }
        else materials[i].bump = "";
    }
    // Endprodukt zurück geben
    return materials;
}
*/