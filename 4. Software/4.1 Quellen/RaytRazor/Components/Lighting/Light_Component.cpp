#include "Light_Component.h"

/**
 * @brief Konstruktor, der die Lichtquelle mit einer Anfangsintensität und -farbe initialisiert.
 *
 * @param intensity Anfangsintensität des Lichts (Standard: 1.0f)
 * @param color Anfangsfarbe des Lichts (Standard: Weiß - glm::vec3(1.0f, 1.0f, 1.0f))
 */
Light_Component::Light_Component(float intensity, glm::vec3 color)
    : intensity(intensity), color(color)
{
}

/**
 * @brief Gibt die Intensität des Lichts zurück.
 *
 * @return float Intensität des Lichts
 */
float Light_Component::getIntensity() const
{
    return intensity;
}

/**
 * @brief Setzt die Intensität des Lichts.
 *
 * @param newIntensity Neue Intensität des Lichts
 */
void Light_Component::setIntensity(float newIntensity)
{
    intensity = newIntensity;
}

/**
 * @brief Gibt die Farbe des Lichts zurück.
 *
 * @return glm::vec3 Farbe des Lichts (RGB)
 */
glm::vec3 Light_Component::getColor() const
{
    return color;
}

/**
 * @brief Setzt die Farbe des Lichts.
 *
 * @param newColor Neue Farbe des Lichts (RGB)
 */
void Light_Component::setColor(const glm::vec3& newColor)
{
    color = newColor;
}

/**
 * @brief Aktualisiert die Eigenschaften des Lichts basierend auf einer Zeitdifferenz.
 * Dies könnte für dynamische Lichteffekte oder Animationen nützlich sein.
 *
 * @param deltaTime Zeitdifferenz, die seit dem letzten Update vergangen ist
 */
void Light_Component::update(float deltaTime)
{

}
