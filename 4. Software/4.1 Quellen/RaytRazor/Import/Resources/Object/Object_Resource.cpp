#include "Object_Resource.h"

//TODO:
// - Object_Resource::Object_Resource:
// -> Prüfen, ob indices und vertices die gleiche größe besitzen.
// -> Überlegen und implementieren, was passiert wenn dem nicht so sein sollte.
// - Object_Resource::update_indices_and_vertices()? Implementieren?

Object_Resource::Object_Resource(const boost::uuids::uuid uuid, const string& path,
                                 const std::vector<int>& indices, const std::vector<Vertex>& vertices)
                                 : Base_Resource(uuid, OBJECT, path)
{
    this->indices = indices;
    this->vertices = vertices;
}

std::vector<int> Object_Resource::get_indices() const
{
    if (indices.size() != 0)
        return this->indices;

    return std::vector<int>();
}

void Object_Resource::set_indices(const std::vector<int>& new_Indices)
{
    if (new_Indices.size() != this->vertices.size())
    {
        Logger::log(MessageType::SEVERE, "Object_Resource::set_indices(): Size of new_Indices does not match size of this->vertices!");
        return;
    }

    this->indices = new_Indices;
}

std::vector<Vertex> Object_Resource::get_vertices() const
{
    if (vertices.size() != 0)
        return this->vertices;

    return std::vector<Vertex>();
}

void Object_Resource::set_vertices(const std::vector<Vertex>& new_Vertices)
{
    if (new_Vertices.size() != this->indices.size())
    {
        Logger::log(MessageType::SEVERE, "Object_Resource::set_vertices(): Size of new_Vertices does not match size of this->indices!");
        return;
    }

    this->vertices = new_Vertices;
}
