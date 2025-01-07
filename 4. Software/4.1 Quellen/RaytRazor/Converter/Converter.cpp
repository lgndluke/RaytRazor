#include "Converter.h"

void Converter::convert_to_matrix_indices(Object_Resource& source)
{
    if (!source.matrix_indices_is_empty())
        return;

    const vector<int> input = source.get_indices();
    MatrixXu output(3,input.size()/3);

    for (int i = 0; i < input.size()/3; i++)
    {
        output.col(i) << input[i], input[i+1], input[i+2];
    }

    source.set_matrix_indices(output);
}

void Converter::convert_to_matrix_vertices(Object_Resource& source)
{
    if (!source.matrix_vertices_is_empty())
        return;

    const vector<Vertex> input = source.get_vertices();
    MatrixXf output(3,input.size()/3);

    for (int i = 0; i < input.size(); ++i)
    {
        output.col(i) << input[i].position.x, input[i].position.y, input[i].position.z;
    }

    source.set_matrix_vertices(output);
}

void Converter::convert_to_matrix_colors(Object_Resource& source)
{
    if (!source.matrix_colors_is_empty())
        return;

    const vector<Vertex> input = source.get_vertices();
    MatrixXf output(3,input.size()/3);

    for (int i = 0; i < input.size(); ++i)
    {
        output.col(i) << input[i].color.x, input[i].color.y, input[i].color.z;
    }

    source.set_matrix_colors(output);
}