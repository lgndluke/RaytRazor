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
    Light_Component(float intensity = 1.0f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

    float getIntensity() const;

    void setIntensity(float newIntensity);

    glm::vec3 getColor() const;

    void setColor(const glm::vec3& newColor);

    void update(float deltaTime);

private:
    float intensity;

    glm::vec3 color;
};

#endif  // LIGHT_COMPONENT_H
