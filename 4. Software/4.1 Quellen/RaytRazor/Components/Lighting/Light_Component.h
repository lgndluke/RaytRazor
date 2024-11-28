#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include "../Base_Component.h"

/**
 * @brief Abgeleitete Klasse Light_Component repräsentiert eine Licht Komponente.
 * @author Christian Kasper, Lukas Jeckle
 */
class Light_Component : public Base_Component
{

    public:

        /**
         * @brief Konstruktor zum Erstellen eines Light_Component Objekts.
         *
         * @param uuid                   UUID des Components.
         * @param name                   Name des Components.
         * @param position               3D-Position des Components.
         * @param rotation               3D-Rotation des Components.
         * @param scale                  3D-Skalierung des Components.
         *
         * @param intensity              Intensität der Lichtstärke.
         * @param color                  Farbeinstellungen der Lichtquelle.
         */
        Light_Component(const boost::uuids::uuid uuid, const string& name,
                        const optional<glm::vec3> position, const optional<glm::vec3> rotation,
                        const optional<glm::vec3> scale, const float intensity, const glm::vec3 color);

        /**
         * @brief Methode, um die Intensität des Light_Components zu erhalten.
         * @return float                 Intensität des Light_Components.
         */
        float get_intensity() const;

        /**
         * @brief Methode, um die Intensität des Light_Components zu ändern.
         * @param new_intensity          Neue Intensität des Light_Components.
         */
        void set_intensity(const float new_intensity);

        /**
         * @brief Methode, um die Farbe des Light_Components zu erhalten.
         * @return glm::vec3             Farbe des Light_Components.
         */
        glm::vec3 get_color() const;

        /**
         * @brief Methode, um die Farbe des Light_Components zu ändern.
         * @param new_color             Neue Farbe des Light_Components.
         */
        void set_color(const glm::vec3 new_color);

    private:

        float intensity;
        glm::vec3 color;

};

#endif
