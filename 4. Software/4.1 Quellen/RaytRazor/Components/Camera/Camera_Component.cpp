#include "Camera_Component.h"

/**
 * @brief Konstruktor zur Initialisierung der Kamera mit Position, Ausrichtung und Projektionsparametern.
 *
 * @param position Die Position der Kamera im 3D-Raum
 * @param front Die Blickrichtung der Kamera
 * @param up Die "Oben"-Richtung der Kamera (zur Bestimmung der Orientierung)
 * @param fov Das Sichtfeld (Field of View) in Grad
 * @param near Die Nah-Clipping-Ebene
 * @param far Die Fern-Clipping-Ebene
 */
Camera_Component::Camera_Component(glm::vec3 position, glm::vec3 front, glm::vec3 up,
                                   float fov, float near, float far)
    : position(position), front(front), up(up), fov(fov), nearClip(near), farClip(far)
{
}

/**
 * @brief Berechnet und gibt die View-Matrix der Kamera zurück.
 * Die View-Matrix beschreibt die Position und Ausrichtung der Kamera im Weltkoordinatensystem.
 *
 * @return glm::mat4 Die View-Matrix
 */
glm::mat4 Camera_Component::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);  // Erstelle die View-Matrix
}

/**
 * @brief Berechnet und gibt die Projektion-Matrix zurück.
 * Die Projektion-Matrix transformiert 3D-Koordinaten in 2D-Bildkoordinaten.
 *
 * @param aspectRatio Das Seitenverhältnis (Breite / Höhe) des Ansichtsfensters
 * @return glm::mat4 Die Projektion-Matrix
 */
glm::mat4 Camera_Component::getProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);  // Perspektivische Projektion
}

/**
 * @brief Aktualisiert die Position der Kamera.
 *
 * @param newPosition Die neue Position der Kamera
 */
void Camera_Component::updateCameraPosition(glm::vec3 newPosition)
{
    position = newPosition;
}

/**
 * @brief Aktualisiert die Ausrichtung der Kamera.
 *
 * @param newFront Die neue Blickrichtung der Kamera
 * @param newUp Die neue "Oben"-Richtung der Kamera
 */
void Camera_Component::updateCameraOrientation(glm::vec3 newFront, glm::vec3 newUp)
{
    front = newFront;
    up = newUp;
}

/**
 * @brief Gibt die aktuelle Position der Kamera zurück.
 *
 * @return glm::vec3 Die Position der Kamera
 */
glm::vec3 Camera_Component::getPosition() const
{
    return position;
}
