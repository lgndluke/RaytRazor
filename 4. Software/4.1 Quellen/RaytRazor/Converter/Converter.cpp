#include "Converter.h"

void Converter::convert_to_matrix_indices(const shared_ptr<Object_Resource>& source)
{
    if (!source->matrix_indices_is_empty())
        return;

    const std::vector<Indice>& input = source->get_indices();

    // Überprüfen, ob der Input leer ist
    if (input.empty())
    {
        Logger::log(MessageType::WARN, "Converter::convert_to_matrix_indices(): Input indices vector is empty.");
        return;
    }

    try
    {
        // Erstellung der MatrixXu für Indizes
        nanogui::MatrixXu output(3, input.size());

        // Befüllung der Matrix
        for (size_t i = 0; i < input.size(); ++i)
        {
            output(0, i) = static_cast<unsigned int>(input[i].vertexIndices_v[0]); //x vertex pos
            output(1, i) = static_cast<unsigned int>(input[i].vertexIndices_v[1]); //y vertex pos
            output(2, i) = static_cast<unsigned int>(input[i].vertexIndices_v[2]); //z vertex pos
        }

        // Setzen der Matrix in `source`
        source->set_matrix_indices(output);
    }
    catch (const std::exception& e)
    {
        Logger::log(MessageType::SEVERE, "Failed to set matrix indices: " + std::string(e.what()));
    }
}

void Converter::convert_to_matrix_vertices(const shared_ptr<Object_Resource>& object_resource, const shared_ptr<Material_Resource>& material_resource)
{
    if (!object_resource->matrix_vertices_is_empty())
        return;

    std::vector<Vertex> vertices = object_resource->get_vertices();
    for (auto & vertex : vertices)
    {
        string v_material_name = vertex.materialName;
        std::vector<Material> materials = material_resource->get_materials();

        for (auto & material : materials)
        {
            if (material.name == v_material_name)
            {
                vertex.color = material.diffuse;
            }
        }
    }
    object_resource->set_vertices(vertices);

    Eigen::MatrixXf output(3, vertices.size());
    for (size_t i = 0; i < vertices.size(); i++)
    {
        output.col(i) << vertices[i].position.x, vertices[i].position.y, vertices[i].position.z;
    }

    object_resource->set_matrix_vertices(output);
}

void Converter::convert_to_matrix_colors(const shared_ptr<Object_Resource>& object_resource,  const shared_ptr<Material_Resource>& material_resource)
{
    const auto& vertices = object_resource->get_vertices();

    Eigen::MatrixXf output(3, vertices.size());
    for (size_t i=0; i<vertices.size(); ++i)
    {
        output.col(i) << vertices[i].color.x, vertices[i].color.y, vertices[i].color.z;
    }

    material_resource->set_matrix_colors(output);
}

Eigen::Matrix4f Converter::convert_from_GLM_to_EigenMatrix(glm::mat4 source)
{
    Eigen::Matrix4f eigenMatrix;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            eigenMatrix(row, col) = source[col][row];
        }
    }
    return eigenMatrix;
}
