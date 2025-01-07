#ifndef CONVERTER_H
#define CONVERTER_H

#include "../Import/Resources/Object/Object_Resource.h"
#include <nanogui/nanogui.h>

using namespace std;
using namespace nanogui;

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
    static void convert_to_matrix_indices(Object_Resource& source);

    /**
     * @brief Methode, um die Vertices-Matrix einer Object_Resource zu füllen.
     * @param source                Die betreffende Object_Resource.
     */
    static void convert_to_matrix_vertices(Object_Resource& source);

    /**
     * @brief Methode, um die Colors-Matrix einer Object_Resource zu füllen.
     * @param source                Die betreffende Object_Resource.
     */
    static void convert_to_matrix_colors(Object_Resource& source);

};

#endif