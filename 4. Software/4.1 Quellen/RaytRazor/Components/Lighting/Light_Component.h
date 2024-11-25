#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include <glm/glm.hpp>                      // Für glm::vec3
#include "../Base_Component.h"              // Annahme: Header für Base_Component

/**
 * @brief Repräsentiert eine Lichtquelle in der Szene mit Eigenschaften wie Intensität und Farbe.
 * Diese Klasse kann für Lichteffekte verwendet werden, die auf ein Objekt in einer 3D-Szene angewendet werden.
 *
 * @author Christian Kasper
 */
class Light_Component : public Base_Component
{

    public:

        /**
         * @brief Konstruktor, der die Lichtquelle mit einer Anfangsintensität und -farbe initialisiert.
         *
         * @param intensity        Anfangsintensität des Lichts (Standard: 1.0f)
         * @param color            Anfangsfarbe des Lichts (Standard: Weiß - glm::vec3(1.0f, 1.0f, 1.0f))
         */
        Light_Component(float intensity = 1.0f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

        /**
         * @brief Gibt die Intensität des Lichts zurück.
         *
         * @return float           Intensität des Lichts
         */
        float getIntensity() const;

        /**
         * @brief Setzt die Intensität des Lichts.
         *
         * @param newIntensity     Neue Intensität des Lichts
         */
        void setIntensity(float newIntensity);

        /**
         * @brief Gibt die Farbe des Lichts zurück.
         *
         * @return glm::vec3       Farbe des Lichts (RGB)
         */
        glm::vec3 getColor() const;

        /**
         * @brief Setzt die Farbe des Lichts.
         *
         * @param newColor         Neue Farbe des Lichts (RGB)
         */
        void setColor(const glm::vec3& newColor);

        /**
         * @brief Aktualisiert die Eigenschaften des Lichts basierend auf einer Zeitdifferenz.
         * Könnte für dynamische Lichteffekte oder Animationen nützlich sein.
         *
         * @param deltaTime        Zeitdifferenz, die seit dem letzten Update vergangen ist
         */
        void update(float deltaTime);

    private:

        float intensity;
        glm::vec3 color;

};

#endif
