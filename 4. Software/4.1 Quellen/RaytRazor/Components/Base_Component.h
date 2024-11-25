#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

#include <string>
#include <glm/glm.hpp>
#include <boost/uuid/uuid.hpp>              // UUID Klasse
#include <boost/uuid/uuid_generators.hpp>   // UUID Generatoren
#include <boost/uuid/uuid_io.hpp>           // Streaming-Operatoren, etc...

using namespace std;

/**
 * @brief Abstrakte Basis Klasse eines Component Objekts.
 * @author Christian Kasper
 */
class Base_Component
{

   public:

       /**
        * @brief Konstruktor, der ein Base_Component Objekt mit zufällig generierter UUID erstellt.
        */
       Base_Component();

       //virtual ~Base_Component();

       /**
        * @brief Methode um die UUID des Components zu erhalten.
        * @return boost::uuids::uuid       UUID des Components.
        */
       boost::uuids::uuid get_uuid() const;

       /**
        * @brief Methode um die Position des Components zu erhalten.
        * @return glm::vec3                Position des Components.
        */
       glm::vec3 get_position() const;

       /**
        * @brief Methode um die Rotation des Components zu erhalten.
        * @return glm::vec3                Rotation des Components.
        */
       glm::vec3 get_rotation() const;

       /**
        * @brief Methode um die Skalierung des Components zu erhalten.
        * @return glm::vec3                Skalierung des Components.
        */
       glm::vec3 get_scale() const;

   private:

       boost::uuids::uuid uuid;
       glm::vec3 position {0.0f, 0.0f, 0.0f};
       glm::vec3 rotation {0.0f, 0.0f, 0.0f};
       glm::vec3 scale {1.0f, 1.0f, 1.0f};

};

#endif
