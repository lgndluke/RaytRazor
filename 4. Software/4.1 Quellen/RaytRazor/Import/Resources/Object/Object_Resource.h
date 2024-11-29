#ifndef OBJECT_RESOURCE_H
#define OBJECT_RESOURCE_H

#include "../Base_Resource.h"
#include "../../../Utility/Logger/Logger.h"
#include <glm/glm.hpp>
#include <nanogui/glutil.h>

using namespace std;

/**
 * @brief Struktur Vertex repräsentiert den Aufbau eines Vertex Objekts.
 * @author Leon Musliu
 */
struct Vertex
{
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
 * @author Lukas Jeckle
 */
class Object_Resource : Base_Resource
{

    public:

        /**
         * @brief Konstruktor zum Erstellen eines Object_Resource Objekts.
         *
         * @param uuid                   UUID der Ressource.
         * @param path                   Pfad der Ressource.
         *
         * @param indices                Indices der Object_Resource.
         * @param vertices               Vertices der Object_Resource.
         */
        Object_Resource(const boost::uuids::uuid uuid, const string& path,
                        const std::vector<int>& indices, const std::vector<Vertex>& vertices);

        /**
         * @brief Methode, um die Indices der Object_Resource zu erhalten.
         * @return std::vector<int>      Die Indices der Object_Resource, falls diese vorhanden sind. Andernfalls, einen leeren std::vector<int>() Vektor.
         */
        [[nodiscard]] std::vector<int> get_indices() const;

        /**
         * @brief Methode, um die Indices der Object_Resource zu ändern.
         * @param new_Indices           Neue Indices der Objekt_Resource.
         */
        void set_indices(const std::vector<int>& new_Indices);

        /**
         * @brief Methode, um die Vertices der Object_Resource zu erhalten.
         * @return std::vector<Vertex>   Die Vertices der Object_Resource, falls diese vorhanden sind. Andernfalls, einen leeren std::vector<Vertices>() Vektor.
         */
        [[nodiscard]] std::vector<Vertex> get_vertices() const;

        /**
         * @brief Methode, um die Vertices der Objekt_Resource zu ändern.
         * @param new_Vertices           Neue Vertices der Object_Resource.
         */
        void set_vertices(const std::vector<Vertex>& new_Vertices);

    private:

        vector<int> indices;
        vector<Vertex> vertices;

};

#endif
