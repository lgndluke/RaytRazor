#ifndef OBJECT_RESOURCE_H
#define OBJECT_RESOURCE_H

#include "../Base_Resource.h"
#include "../../../Utility/Logger/Logger.h"
#include <glm/glm.hpp>
#include <vector>
#include "glad/glad.h"
#include "nanogui/common.h"

using namespace std;

/**
 * @brief Struktur Vertex repräsentiert den Aufbau eines Vertex Objekts.
 * @author Leon Musliu
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
    GLint smoothness;
    string objectName;
    string groupName;
    string materialName;
    string mtlFileName;
};

/**
 * @brief Abgeleitete Klasse Object_Resource repräsentiert eine Objekt Ressource.
 */
class Object_Resource : public Base_Resource {

public:
    /**
     * @brief Konstruktor zum Erstellen eines Object_Resource Objekts.
     *
     * @param uuid                   UUID der Ressource.
     * @param path                   Pfad der Ressource.
     * @param indices                Indices der Object_Resource.
     * @param vertices               Vertices der Object_Resource.
     */
    Object_Resource(const boost::uuids::uuid uuid, const string& path,
                    const std::vector<int>& indices, const std::vector<Vertex>& vertices);

    Object_Resource() = default;

    /**
     * @brief Methode, um die Indices der Object_Resource zu erhalten.
     * @return std::vector<int>      Die Indices der Object_Resource.
     */
    [[nodiscard]] std::vector<int> get_indices() const;

    /**
     * @brief Methode, um die Indices der Object_Resource zu ändern.
     * @param new_indices           Neue Indices der Objekt_Resource.
     */
    void set_indices(const std::vector<int>& new_indices);

    /**
     * @brief Methode, um die Vertices der Object_Resource zu erhalten.
     * @return std::vector<Vertex>   Die Vertices der Object_Resource.
     */
    [[nodiscard]] std::vector<Vertex> get_vertices() const;

    /**
     * @brief Methode, um die Vertices der Objekt_Resource zu ändern.
     * @param new_vertices           Neue Vertices der Object_Resource.
     */
    void set_vertices(const std::vector<Vertex>& new_vertices);

    /**
     * @brief Methode, um die Matrix der Indices zu erhalten.
     * @return const nanogui::MatrixXu& Die Indices-Matrix der Object_Resource.
     */
    [[nodiscard]] const nanogui::MatrixXu& get_matrix_indices() const;

    /**
     * @brief Methode, um die Matrix der Indices zu setzen.
     * @param new_matrix_indices    Neue Matrix der Indices der Object_Resource.
     */
    void set_matrix_indices(const nanogui::MatrixXu& new_matrix_indices);

    /**
     * @brief Methode, um zu überprüfen, ob die Matrix der Indices leer ist.
     * @return bool                 True, falls die Matrix leer ist. Andernfalls, False.
     */
    bool matrix_indices_is_empty() const;

    /**
     * @brief Methode, um die Matrix der Vertices zu erhalten.
     * @return const nanogui::MatrixXf& Die Vertices-Matrix der Object_Resource.
     */
    [[nodiscard]] const nanogui::MatrixXf& get_matrix_vertices() const;

    /**
     * @brief Methode, um die Matrix der Vertices zu setzen.
     * @param new_matrix_vertices   Neue Matrix der Vertices der Object_Resource.
     */
    void set_matrix_vertices(const nanogui::MatrixXf& new_matrix_vertices);

    /**
     * @brief Methode, um zu überprüfen, ob die Matrix der Vertices leer ist.
     * @return bool                 True, falls die Matrix leer ist. Andernfalls, False.
     */
    bool matrix_vertices_is_empty() const;

    /**
     * @brief Methode, um die Matrix der Colors zu erhalten.
     * @return const nanogui::MatrixXf& Die Colors-Matrix der Object_Resource.
     */
    [[nodiscard]] const nanogui::MatrixXf& get_matrix_colors() const;

    /**
     * @brief Methode, um die Matrix der Colors zu setzen.
     * @param new_matrix_colors     Neue Matrix der Colors der Object_Resource.
     */
    void set_matrix_colors(const nanogui::MatrixXf& new_matrix_colors);

    /**
     * @brief Methode, um zu überprüfen, ob die Matrix der Colors leer ist.
     * @return bool                 True, falls die Matrix leer ist. Andernfalls, False.
     */
    bool matrix_colors_is_empty() const;

private:
    std::vector<int> indices;
    std::vector<Vertex> vertices;

    nanogui::MatrixXu matrix_indices;
    nanogui::MatrixXf matrix_vertices;
    nanogui::MatrixXf matrix_colors;
};

#endif
