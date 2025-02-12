#ifndef CONVERTER_H
#define CONVERTER_H

#include "../Import/Resources/Object/Object_Resource.h"
#include "../Import/Resources/Material/Material_Resource.h"
#include <nanogui/nanogui.h>

using namespace std;

/**
 * @brief Converter Klasse, um die eingelesenen Daten einer Object_Resource in die von @link nanogui @endlink geforderten Matrizen umzuwandeln.
 * @author Dennis Welsch, Lukas Jeckle
 */
class Converter {

public:

    /**
     * @brief Statische Klasse ohne Konstruktor.
     */
    Converter() = delete;

    /**
     * @brief Methode, um die Indices-Matrix einer Object_Resource zu füllen.
     * @param source                Die betreffende Object_Resource.
     */
    static void convert_to_matrix_indices(const shared_ptr<Object_Resource>& source);

    /**
     * @brief Methode, um die Vertices-Matrix einer Object_Resource zu füllen.
     * @param object_resource       Die betreffende Object_Resource.
     * @param material_resource     Die betreffende Material_Resource.
     */
    static void convert_to_matrix_vertices(const shared_ptr<Object_Resource>& object_resource, const shared_ptr<Material_Resource>& material_resource);

    /**
     * @brief Methode, um die Colors-Matrix einer Object_Resource zu füllen.
     * @param object_resource       Die betreffende Object_Resource.
     * @param material_resource     Die betreffende Material_Resource.
     */
    static void convert_to_matrix_colors(const shared_ptr<Object_Resource>& object_resource, const shared_ptr<Material_Resource>& material_resource);

    static Eigen::Matrix4f convert_from_GLM_to_EigenMatrix(glm::mat4 source);

};

#endif