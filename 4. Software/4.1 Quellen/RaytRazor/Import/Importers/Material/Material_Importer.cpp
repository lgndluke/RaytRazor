#include "Material_Importer.h"


std::optional<Material_Resource> Material_Importer::import_Material(const boost::uuids::uuid& uuid,
                                                                    const string& path_to_file)
{

    if (!validate_extension(path_to_file, ".mtl"))
    {
        Logger::log(MessageType::SEVERE, "Material_Importer::import_Material(): Type miss-match -> no '.mtl' file: " + path_to_file);
        return std::nullopt;
    }

    if (std::ifstream file(path_to_file); !file.is_open())
    {
        Logger::log(MessageType::SEVERE, "Material_Importer::import_Material(): Unable to open material file: " + path_to_file);
        return std::nullopt;
    }

    //const vector<int> indices = create_Indices(uuid, path_to_file);
    const vector<Material> materials = fetch_materials(path_to_file);

    Material_Resource return_Resource(uuid, path_to_file, materials);
    return return_Resource;

}

std::vector<Material> Material_Importer::fetch_materials(const string& path_to_file)
{
    std::vector<Material> materials;
    std::ifstream file(path_to_file);
    std::string line;
    std::stringstream ss;
    std::string prefix;
    std::string word;
    glm::vec3 matXYZ;
    GLfloat matVal;
    int mtlNR = -1;

    while (std::getline(file, line))
    {

        //Prefix holen
        if(line.empty()) continue;
        ss.clear();
        ss.str(line);
        ss >> prefix;

        // Erstellt einen neuen Material-Eintrag in der Liste, und befüllt diesen.
        if (prefix == "newmtl")
        {
            mtlNR += 1;
            ss >> word;
            materials.emplace_back();
            materials.at(mtlNR).name = word;
        }
        else if (prefix == "Ka")
        {
            ss >> matXYZ.x >> matXYZ.y >> matXYZ.z;
            materials.at(mtlNR).ambient = matXYZ;
        }
        else if (prefix == "Kd")
        {
            ss >> matXYZ.x >> matXYZ.y >> matXYZ.z;
            materials.at(mtlNR).diffuse = matXYZ;
        }
        else if (prefix == "Ke")
        {
            ss >> matXYZ.x >> matXYZ.y >> matXYZ.z;
            materials.at(mtlNR).emissive = matXYZ;
        }
        else if (prefix == "Ks")
        {
            ss >> matXYZ.x >> matXYZ.y >> matXYZ.z;
            materials.at(mtlNR).specular = matXYZ;
        }
        else if (prefix == "Ns")
        {
            ss >> matVal;
            materials.at(mtlNR).shininess = matVal;
        }
        else if (prefix == "d")
        {
            ss >> matVal;
            materials.at(mtlNR).transparency = matVal;
        }
        else if (prefix == "Ni")
        {
            ss >> matVal;
            materials.at(mtlNR).opacity = matVal;
        }
        else if (prefix == "illum")
        {
            ss >> matVal;
            materials.at(mtlNR).illumination = matVal;
        }
        else if (prefix == "map_Ka")
        {
            ss >> word;
            materials.at(mtlNR).map_Ka = word;
        }
        else if (prefix == "map_Kd")
        {
            ss >> word;
            materials.at(mtlNR).map_Kd = word;
        }
        else if (prefix == "map_Ks")
        {
            ss >> word;
            materials.at(mtlNR).map_Ks = word;
        }
        else if (prefix == "map_d")
        {
            ss >> word;
            materials.at(mtlNR).map_d = word;
        }
        else if (prefix == "bump")
        {
            ss >> word;
            materials.at(mtlNR).bump = word;
        }
        else if (prefix == "#")
        {
            continue;
        }
        else
        {
            Logger::log(MessageType::SEVERE, "Material_Importer::fetch_materials(): Unknown Prefix: '" + prefix + "'!");
        }

    }

    return materials;

}

bool Material_Importer::validate_extension(const string &path_to_file, const string &suffix)
{
    if (path_to_file.size() < suffix.size())
        return false;

    return path_to_file.substr(path_to_file.size() - suffix.size()) == suffix;
}
