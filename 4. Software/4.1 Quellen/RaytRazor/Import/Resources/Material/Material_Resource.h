#ifndef MATERIAL_RESOURCE_H
#define MATERIAL_RESOURCE_H

#include "../Base_Resource.h"
#include "../../../Utility/Logger/Logger.h"
#include <glm/glm.hpp>
#include <nanogui/glutil.h>

using namespace std;

/**
 * @brief Struktur Material repräsentiert den Aufbau eines Material Objekts.
 * @author Leon Musliu
 */
struct Material {
    string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat illumination;
    GLfloat transparency;
    GLfloat shininess;
    GLfloat opacity;
    string map_Ka;
    string map_Kd;
    string map_Ks;
    string map_d;
    string bump;
};

/**
 * @brief Abgeleitete Klasse Material_Resource repräsentiert eine Material Ressource.
 * @author Lukas Jeckle
 */
class Material_Resource : public Base_Resource {

public:

    /**
     * @brief Konstruktor zum Erstellen eines Material_Resource Objekts.
     *
     * @param uuid                   UUID der Ressource.
     * @param path                   Pfad der Ressource.
     *
     * @param indices                Indices der Material_Resource.
     * @param materials              Materials der Material_Resource.
     */
    Material_Resource(const boost::uuids::uuid uuid, const string &path, const std::vector<Material> &materials);

    /**
     * @brief Methode, um die Indices der Material_Resource zu erhalten.
     * @return std::vector<int>      Die Indices der Material_Resource, falls diese vorhanden sind. Andernfalls, einen leeren std::vector<int>() Vektor.
     */
    [[nodiscard]] std::vector<int> get_indices() const;

    /**
     * @brief Methode, um die Indices der Material_Resource zu ändern.
     * @param new_Indices             Neue Indices der Material_Resource.
     */
    void set_indices(const std::vector<int> &new_Indices);

    /**
     * @brief Methode, um die Materials der Material_Resource zu erhalten.
     * @return std::vector<Material>  Die Materials der Material_Resource, falls diese vorhanden sind. Andernfalls, einen leeren std::vector<Material>() Vektor.
     */
    [[nodiscard]] std::vector<Material> get_materials() const;

    /**
     * @brief Methode, um die Materials der Material_Resource zu ändern.
     * @param new_Materials           Neue Materials der Material_Resource.
     */
    void set_materials(const std::vector<Material> &new_Materials);

    /**
    * @brief Methode, um die Matrix der Colors zu erhalten.
    * @return const nanogui::MatrixXf& Die Colors-Matrix der Object_Resource.
    */
    [[nodiscard]] const nanogui::MatrixXf &get_matrix_colors() const;

    /**
     * @brief Methode, um die Matrix der Colors zu setzen.
     * @param new_matrix_colors     Neue Matrix der Colors der Object_Resource.
     */
    void set_matrix_colors(const nanogui::MatrixXf &new_matrix_colors);

    /**
     * @brief Methode, um zu überprüfen, ob die Matrix der Colors leer ist.
     * @return bool                 True, falls die Matrix leer ist. Andernfalls, False.
     */
    bool matrix_colors_is_empty() const;


private:

    vector<Material> materials;
    nanogui::MatrixXf matrix_colors;

};

#endif
