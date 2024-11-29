#include "Material_Resource.h"

//TODO:
// - Material_Resource::Material_Resource:
// -> Prüfen, ob indices und materials die gleiche größe besitzen.
// -> Überlegen und implementieren, was passiert wenn dem nicht so sein sollte.
// - Material_Resource::update_indices_and_vertices()? Implementieren?

Material_Resource::Material_Resource(const boost::uuids::uuid uuid, const string& path,
                                     const std::vector<int>& indices, const std::vector<Material>& materials)
                                     : Base_Resource(uuid, MATERIAL, path)
{
    this->indices = indices;
    this->materials = materials;
}

std::vector<int> Material_Resource::get_indices() const
{
    if (indices.size() != 0)
        return this->indices;

    return std::vector<int>();
}

void Material_Resource::set_indices(const std::vector<int>& new_Indices)
{
    if (new_Indices.size() != this->materials.size())
    {
        Logger::log(MessageType::SEVERE, "Material_Resource::set_indices(): Size of new_Indices does not match size of this->materials!");
        return;
    }

    this->indices = new_Indices;
}

std::vector<Material> Material_Resource::get_materials() const
{
    if (materials.size() != 0)
        return this->materials;

    return std::vector<Material>();
}

void Material_Resource::set_materials(const std::vector<Material>& new_Materials)
{
    if (new_Materials.size() != this->indices.size())
    {
        Logger::log(MessageType::SEVERE, "Material_Resource::set_materials(): Size of new_Materials does not match size of this->indices!");
        return;
    }

    this->materials = new_Materials;
}
