#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "../Base_Component.h"

using namespace std;

/**
 * @brief Abgeleitete Klasse Render_Component repräsentiert eine Render Komponente.
 * @author Christian Kasper, Lukas Jeckle
 */
class Render_Component : public Base_Component
{

    public:

        ~Render_Component() override = default;

        [[nodiscard]] ComponentType get_type() const {
           return RENDER;
        }

        /**
         * @brief Konstruktor zum Erstellen eines Render_Component Objekts.
         *
         * @param uuid                      UUID des Components.
         * @param name                      Name des Components.
         * @param position                  3D-Position des Components.
         * @param rotation                  3D-Rotation des Components.
         * @param scale                     3D-Skalierung des Components.
         *
         * @param object_UUID               UUID der verwendeten Object_Resource.
         * @param material_UUID             UUID der verwendeten Material_Resource.
         */
        Render_Component(const boost::uuids::uuid uuid, const string& name,
                         const optional<glm::vec3> position, const optional<glm::vec3> rotation,
                         const optional<glm::vec3> scale, const boost::uuids::uuid object_UUID,
                         const boost::uuids::uuid material_UUID);

        /**
         * @brief Methode, um die UUID der verwendeten Object_Resource zu erhalten.
         * @return boost::uuids::uuid       UUID der verwendeten Object_Resource.
         */
        [[nodiscard]] boost::uuids::uuid get_object_UUID() const;

        /**
         * @brief Methode, um die UUID der verwendeten Object_Resource zu ändern.
         * @param new_UUID                  Neue UUID der verwendeten Object_Resource.
         */
        void set_object_UUID(const boost::uuids::uuid new_UUID);

        /**
         * @brief Methode, um die UUID der verwendeten Material_Resource zu erhalten.
         * @return boost::uuids::uuid       UUID der verwendeten Material_Resource.
         */
        [[nodiscard]] boost::uuids::uuid get_material_UUID() const;

        /**
         * @brief Methode, um die UUID der verwendeten Material_Resource zu ändern.
         * @param new_UUID                  Neue UUID der verwendeten Material_Resource.
         */
        void set_material_UUID(const boost::uuids::uuid new_UUID);

private:

    boost::uuids::uuid object_UUID;
    boost::uuids::uuid material_UUID;

};

#endif
