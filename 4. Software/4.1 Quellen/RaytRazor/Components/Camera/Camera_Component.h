#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

// Importiere notwendige Bibliotheken
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // Für Matrix-Transformationen
#include <glm/gtc/type_ptr.hpp>  // Für das Arbeiten mit glm-Matrizen
#include "../Base_Component.h"  // Die Basisklasse

using namespace std;

/**
 * @brief Repräsentiert eine Kamera-Komponente für 3D-Rendering
 * Die Kamera hat Position, Ausrichtung und Projektion, die für das Rendering einer Szene notwendig sind.
 *
 * @author Dein Name
 */
class Camera_Component : public Base_Component
{
public:
    // Konstruktor mit Standardwerten
    Camera_Component(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
                     glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
                     glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                     float fov = 45.0f, float near = 0.1f, float far = 100.0f);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void updateCameraPosition(glm::vec3 newPosition);

    void updateCameraOrientation(glm::vec3 newFront, glm::vec3 newUp);

    glm::vec3 getPosition() const;

private:
    glm::vec3 position;  // Position der Kamera
    glm::vec3 front;     // Blickrichtung der Kamera
    glm::vec3 up;        // Obere Achse der Kamera
    float fov;           // Sichtfeld (Field of View)
    float nearClip;      // Nah-Clipping-Ebene
    float farClip;       // Fern-Clipping-Ebene
};

#endif // CAMERA_COMPONENT_H
