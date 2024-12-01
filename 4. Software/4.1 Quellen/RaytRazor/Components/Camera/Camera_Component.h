#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "../Base_Component.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

/**
 * @brief Abgeleitete Klasse Camera_Component repräsentiert eine Kamera Komponente.
 * @author Christian Kasper, Lukas Jeckle
 */
class Camera_Component : public Base_Component
{

    public:

        /**
         * @brief Konstruktor zum Erstellen eines Camera_Component Objekts.
         *
         * @param uuid                   UUID des Components.
         * @param name                   Name des Components.
         * @param position               3D-Position des Components.
         * @param rotation               3D-Rotation des Components.
         * @param scale                  3D-Skalierung des Components.
         *
         * @param fov                    Sichtfeld der Kamera.
         * @param aspect_ratio           Seitenverhältnis der Kamera.
         * @param near_clip              Entfernung, ab der zu nahe Objekte nicht mehr gerendert werden.
         * @param far_clip               Entfernung, ab der zu weit entfernte Objekte nicht mehr gerendert werden.
         */
        Camera_Component(const boost::uuids::uuid uuid, const string& name,
                         const optional<glm::vec3> position, const optional<glm::vec3> rotation,
                         const optional<glm::vec3> scale, const float fov, const float aspect_ratio,
                         const float near_clip, const float far_clip);


        /**
         * @brief Methode, um das FOV des Camera_Components zu erhalten.
         * @return float                FOV des Camera_Components.
         */
        [[nodiscard]] float get_fov() const;

        /**
         * @brief Methode, um das FOV des Camera_Components zu ändern.
         * @param new_fov               Neues FOV des Camera_Components.
         */
        void set_fov(const float new_fov);

        /**
         * @brief Methode, um das Aspekt-Ratio des Camera_Components zu erhalten.
         * @return float                Aspekt-Ratio des Camera_Components.
         */
        [[nodiscard]] float get_aspect_ratio() const;

        /**
         * @brief Methode, um das Aspekt-Ratio des Camera_Components zu ändern.
         * @param new_aspect_ratio      Neues Aspekt-Ratio des Camera_Components.
         */
        void set_aspect_ratio(const float new_aspect_ratio);

        /**
         * @brief Methode, um den Near-Clip des Camera_Components zu erhalten.
         * @return float                Near-Clip des Camera_Components.
         */
        [[nodiscard]] float get_near_clip() const;

        /**
         * @brief Methode, um den Near-Clip des Camera_Components zu ändern.
         * @param new_near_clip         Neuer Near-Clip des Camera_Components.
         */
        void set_near_clip(const float new_near_clip);

        /**
         * @brief Methode, um den Far-Clip des Camera_Components zu erhalten.
         * @return float                Far-Clip des Camera_Components.
         */
        [[nodiscard]] float get_far_clip() const;

        /**
         * @brief Methode, um den Far-Clip des Camera_Components zu ändern.
         * @param new_far_clip          Neuer Far-Clip des Camera_Components.
         */
        void set_far_clip(const float new_far_clip);

        /**
         * @brief Methode, um die View-Matrix der Kamera zu berechnen.
         * @return glm::mat4            Die berechnete View-Matrix.
         */
        [[nodiscard]] glm::mat4 getViewMatrix() const;

        /**
         * @brief Methode, um die Projections-Matrix der Kamera zu berechnen.
         * @return glm::mat4            Die berechnete Projektions-Matrix.
         */
        [[nodiscard]] glm::mat4 getProjectionMatrix() const;

    private:

        float fov;
        float aspect_ratio;
        float near_clip;
        float far_clip;

};

#endif
