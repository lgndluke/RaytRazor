#include "Object_Resource.h"

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

optional<MatrixXu> Object_Resource::get_matrix_indices() const
{
    if (matrix_indices_is_empty())
        return std::nullopt;

    return this->matrix_indices;
}

void Object_Resource::set_matrix_indices(const MatrixXu& new_matrix_indices)
{
    this->matrix_indices = new_matrix_indices;
}

bool Object_Resource::matrix_indices_is_empty() const
{
    if (this->matrix_indices.size() == 0)
        return true;

    return false;
}

optional<MatrixXf> Object_Resource::get_matrix_vertices() const
{
    if (matrix_vertices_is_empty())
        return std::nullopt;

    return this->matrix_vertices;
}

void Object_Resource::set_matrix_vertices(const MatrixXf& new_matrix_vertices)
{
    this->matrix_vertices = new_matrix_vertices;
}

bool Object_Resource::matrix_vertices_is_empty() const
{
    if (this->matrix_vertices.size() == 0)
        return true;

    return false;
}

optional<MatrixXf> Object_Resource::get_matrix_colors() const
{
    if (matrix_colors_is_empty())
        return std::nullopt;

    return this->matrix_colors;
}

void Object_Resource::set_matrix_colors(const MatrixXf& new_matrix_color)
{
    this->matrix_colors = new_matrix_color;
}

bool Object_Resource::matrix_colors_is_empty() const
{
    if (matrix_colors.size() == 0)
        return true;

    return false;
}
