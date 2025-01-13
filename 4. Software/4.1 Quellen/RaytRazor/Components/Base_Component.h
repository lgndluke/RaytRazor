#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

#include <optional>
#include <string>
#include <glm/glm.hpp>
#include <boost/uuid/uuid.hpp>
#include <nlohmann/json.hpp>

using namespace std;

/**
 * @brief Enum das die unterschiedlichen Component Typen definiert.
 *
 * @code CAMERA@endcode - um 'CAMERA' Objekte zu beschreiben.
 * @code RENDER@endcode - um 'RENDER' Objekte zu beschreiben.
 * @code LIGHT@endcode  - um 'LIGHT' Objekte zu beschreiben.
 *
 * @author Lukas Jeckle
 */
enum ComponentType
{
    CAMERA,
    RENDER,
    LIGHT
};

/**
 * @brief Abstrakte Basis Klasse für Component Objekte.
 * @author Christian Kasper, Lukas Jeckle
 */
class Base_Component
{

   public:

       virtual ~Base_Component() = default;

    template<class c> inline bool instanceOf(const Base_Component* base)
    {
        return dynamic_cast <const c*> (base);
    }

       //[[nodiscard]] virtual ComponentType get_type() const = 0;
       /**
        * @brief Konstruktor zum Erstellen eines Base_Component Objekts.
        *
        * @param uuid                   UUID des Components.
        * @param type                   Typ des Components.
        * @param name                   Name des Components.
        * @param position               3D-Position des Components.
        * @param rotation               3D-Rotation des Components.
        * @param scale                  3D-Skalierung des Components.
        */
       Base_Component(const boost::uuids::uuid uuid,
                      const ComponentType type,
                      const string& name,
                      const optional<glm::vec3> position,
                      const optional<glm::vec3> rotation,
                      const optional<glm::vec3> scale);

       Base_Component() = default;

       /**
        * @brief Methode, um die UUID des Components zu erhalten.
        * @return boost::uuids::uuid       UUID des Components.
        */
       [[nodiscard]] boost::uuids::uuid get_uuid() const;

       /**
        * @brief Methode, um den Typ des Components zu erhalten.
        * @return ComponentType            Typ des Components.
        */
       [[nodiscard]] ComponentType get_type() const;

        /**
         * @brief Methode, um den Namen des Components zu erhalten.
         * @return string                   Name des Components.
         */
       [[nodiscard]] string get_name() const;

        /**
         * @brief Methode, um den Namen des Components zu ändern.
         * @param new_Name                  Neuer Name des Components.
         */
       void set_name(const string& new_Name);

       /**
        * @brief Methode, um die Position des Components zu erhalten.
        * @return glm::vec3                Position des Components, falls diese gesetzt ist. Andernfalls glm::vec3(0, 0, 0).
        */
       [[nodiscard]] glm::vec3 get_position() const;


       /**
        * @brief Methode, um die Position des Components zu ändern.
        * @param new_Position              Neue Position des Components.
        */
       void set_position(glm::vec3 new_Position);

       /**
        * @brief Methode um die Rotation des Components zu erhalten.
        * @return glm::vec3                Rotation des Components, falls diese gesetzt ist. Andernfalls glm::vec3(0.0f, 0.0f, 0.0f).
        */
       [[nodiscard]] glm::vec3 get_rotation() const;

       /**
        * @brief Methode, um die Rotation des Components zu ändern.
        * @param new_Rotation               Neue Rotation des Components.
        */
       void set_rotation(glm::vec3 new_Rotation);

       /**
        * @brief Methode um die Skalierung des Components zu erhalten.
        * @return glm::vec3                Skalierung des Components, falls diese gesetzt ist. Andernfalls glm::vec3(0.0f, 0.0f, 0.0f).
        */
       [[nodiscard]] glm::vec3 get_scale() const;

        /**
         * @brief Methode, um die Skalierung des Components zu ändern.
         * @param new_Scale                Neue Skalierung des Components.
         */
       void set_scale(glm::vec3 new_Scale);

   private:

       boost::uuids::uuid uuid;
       ComponentType type;
       string name;
       optional<glm::vec3> position;
       optional<glm::vec3> rotation;
       optional<glm::vec3> scale;

};

#endif
