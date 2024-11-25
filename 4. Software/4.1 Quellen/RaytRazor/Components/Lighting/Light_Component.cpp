#include "Light_Component.h"

//TODO:
// - Implementieren der Light_Component::update(float deltaTime) Methode.

Light_Component::Light_Component(float intensity, glm::vec3 color)
    : intensity(intensity), color(color)
{
}

float Light_Component::getIntensity() const
{
    return intensity;
}

void Light_Component::setIntensity(float newIntensity)
{
    intensity = newIntensity;
}

glm::vec3 Light_Component::getColor() const
{
    return color;
}

void Light_Component::setColor(const glm::vec3& newColor)
{
    color = newColor;
}

void Light_Component::update(float deltaTime)
{
    // Methode implementieren.
}
