#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include <glm/glm.hpp>  // Für glm::vec3
#include "../Base_Component.h"  // Annahme: Header für Base_Component

/**
 * @brief Repräsentiert eine Lichtquelle in der Szene mit Eigenschaften wie Intensität und Farbe.
 * Diese Klasse kann für Lichteffekte verwendet werden, die auf ein Objekt in einer 3D-Szene angewendet werden.
 *
 * @author Dein Name
 */
class Light_Component : public Base_Component
{
public:
    // Konstruktor mit Standardwerten für Intensität und Farbe
    Light_Component(float intensity = 1.0f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

    // Getter für Intensität
    float getIntensity() const;

    // Setter für Intensität
    void setIntensity(float newIntensity);

    // Getter für Farbe
    glm::vec3 getColor() const;

    // Setter für Farbe
    void setColor(const glm::vec3& newColor);

    // Methode zur Aktualisierung der Licht-Eigenschaften (kann für dynamische Lichter nützlich sein)
    void update(float deltaTime);

private:
    // Lichtintensität (typischerweise ein Wert zwischen 0 und 1)
    float intensity;

    // Lichtfarbe, dargestellt als vec3 (RGB-Werte)
    glm::vec3 color;
};

#endif  // LIGHT_COMPONENT_H
