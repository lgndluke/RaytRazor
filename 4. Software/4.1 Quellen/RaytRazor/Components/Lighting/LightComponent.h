#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

//TODO Add imports here.

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */

/**
 * @brief Repräsentiert eine Lichtquelle im 3D-Raum.
 */
struct LightComponent {
    float intensity;
    glm::vec3 color{0.0f, 0.0f, 0.0f}; // RGB-Farbe des Lichts
};

#endif
