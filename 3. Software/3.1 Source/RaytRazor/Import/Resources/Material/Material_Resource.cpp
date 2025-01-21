#include "Material_Resource.h"

//TODO:
// - Material_Resource::Material_Resource:
// -> Prüfen, ob indices und materials die gleiche größe besitzen.
// -> Überlegen und implementieren, was passiert wenn dem nicht so sein sollte.
// - Material_Resource::update_indices_and_materials()? Implementieren?

Material_Resource::Material_Resource(const boost::uuids::uuid uuid, const string& path, const std::vector<Material>& materials)
                                     : Base_Resource(uuid, MATERIAL, path)
{
    this->materials = materials;
}

std::vector<Material> Material_Resource::get_materials() const
{
    if (!materials.empty())
        return this->materials;

    return {};
}

void Material_Resource::set_materials(const std::vector<Material>& new_Materials)
{
    /*
    if (new_Materials.size() != this->indices.size())
    {
        Logger::log(MessageType::SEVERE, "Material_Resource::set_materials(): Size of new_Materials does not match size of this->indices!");
        return;
    }
     */

    this->materials = new_Materials;
}

const nanogui::MatrixXf &Material_Resource::get_matrix_colors() const {
    return this->matrix_colors; // Direkte Rückgabe
}

void Material_Resource::set_matrix_colors(const nanogui::MatrixXf &new_matrix_colors) {
    if (new_matrix_colors.rows() != 3) {
        Logger::log(MessageType::SEVERE, "Object_Resource::set_matrix_colors(): Matrix must have 3 rows for valid colors!");
        return;
    }

    this->matrix_colors = new_matrix_colors;
}

bool Material_Resource::matrix_colors_is_empty() const {
    return this->matrix_colors.cols() == 0; // Direkte Prüfung auf leere Matrix
}
