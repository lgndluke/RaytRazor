#ifndef ENTITY_H
#define ENTITY_H
#include "Basis/Components.h"
#include <glm/glm.hpp>

//TODO Add imports here.

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */

struct Position {
    glm::vec3 pos{0.0f, 0.0f, 0.0f};
};

/**
 * @brief Repräsentiert eine Rotation im 3D-Raum (Winkel in Grad).
 */
struct Rotation {
    glm::vec3 rot{0.0f, 0.0f, 0.0f};
};

/**
 * @brief Repräsentiert eine Skalierung im 3D-Raum.
 */
struct Scale {
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
};

/**
 * @brief Transformation einer Entität, beinhaltet Position, Rotation und Skalierung.
 */
struct Translation {
    Position position;
    Rotation rotation;
    Scale scale;
};

/**
 * @brief Entitätsstruktur mit eindeutiger ID und Transformation.
 */
struct Entity {
    int uuid;
    Translation translation;
    Components components;
};

#endif
