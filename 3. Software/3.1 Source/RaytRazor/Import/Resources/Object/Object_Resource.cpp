#include "Object_Resource.h"

Object_Resource::Object_Resource(const boost::uuids::uuid uuid, const string& path,
                                 const std::vector<Indice>& indices, const std::vector<Vertex>& vertices)
                                 : Base_Resource(uuid, OBJECT, path)
{
    this->indices = indices;
    this->vertices = vertices;
}

std::vector<Indice> Object_Resource::get_indices() const
{
    return this->indices; // Direkte Rückgabe
}

void Object_Resource::set_indices(const std::vector<Indice>& new_indices)
{
    /* nichtmehr benötigt
    if (new_indices.size() % 3 != 0) {
        Logger::log(MessageType::SEVERE, "Object_Resource::set_indices(): Size of new_indices is not a multiple of 3!");
        return;
    }
    */
    this->indices = new_indices;
}

std::vector<Vertex> Object_Resource::get_vertices() const
{
    return this->vertices; // Direkte Rückgabe
}

void Object_Resource::set_vertices(const std::vector<Vertex>& new_vertices)
{
    this->vertices = new_vertices;
}

const nanogui::MatrixXu& Object_Resource::get_matrix_indices() const
{
    return this->matrix_indices; // Direkte Rückgabe
}

void Object_Resource::set_matrix_indices(const nanogui::MatrixXu& new_matrix_indices)
{
    /* nichtmehr benötigt
    if (new_matrix_indices.rows() != 3) {
        Logger::log(MessageType::SEVERE, "Object_Resource::set_matrix_indices(): Matrix must have 3 rows for valid indices!");
        return;
    }
    */
    this->matrix_indices = new_matrix_indices;
}

bool Object_Resource::matrix_indices_is_empty() const
{
    return this->matrix_indices.cols() == 0; // Direkte Prüfung auf leere Matrix
}

const nanogui::MatrixXf& Object_Resource::get_matrix_vertices() const
{
    return this->matrix_vertices; // Direkte Rückgabe
}

void Object_Resource::set_matrix_vertices(const nanogui::MatrixXf& new_matrix_vertices)
{
    if (new_matrix_vertices.rows() != 3) {
        Logger::log(MessageType::SEVERE, "Object_Resource::set_matrix_vertices(): Matrix must have 3 rows for valid vertices!");
        return;
    }

    this->matrix_vertices = new_matrix_vertices;
}

bool Object_Resource::matrix_vertices_is_empty() const
{
    return this->matrix_vertices.cols() == 0; // Direkte Prüfung auf leere Matrix
}