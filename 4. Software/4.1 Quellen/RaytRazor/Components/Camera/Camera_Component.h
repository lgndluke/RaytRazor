#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>     // Für Matrix-Transformationen
#include <glm/gtc/type_ptr.hpp>             // Für das Arbeiten mit glm-Matrizen
#include "../Base_Component.h"              // Die Basisklasse

using namespace std;

/**
 * @brief Repräsentiert eine Kamera-Komponente für 3D-Rendering.
 * Die Kamera hat Position, Ausrichtung und Projektion, die für das Rendering einer Szene notwendig sind.
 *
 * @author Christian Kasper
 */
class Camera_Component : public Base_Component
{

    public:

        /**
         * @brief Konstruktor zur Initialisierung der Kamera mit Position, Ausrichtung und Projektionsparametern.
         *
         * @param position         Die Position der Kamera im 3D-Raum
         * @param front            Die Blickrichtung der Kamera
         * @param up               Die "Oben"-Richtung der Kamera (zur Bestimmung der Orientierung)
         * @param fov              Das Sichtfeld (Field of View) in Grad
         * @param near             Die Nah-Clipping-Ebene
         * @param far              Die Fern-Clipping-Ebene
         */
        Camera_Component(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
                         glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
                         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                         float fov = 45.0f, float near = 0.1f, float far = 100.0f);

        /**
         * @brief Berechnet und gibt die View-Matrix der Kamera zurück.
         * Die View-Matrix beschreibt die Position und Ausrichtung der Kamera im Weltkoordinatensystem.
         *
         * @return glm::mat4       Die View-Matrix
         */
        glm::mat4 getViewMatrix() const;

        /**
         * @brief Berechnet und gibt die Projektion-Matrix zurück.
         * Die Projektion-Matrix transformiert 3D-Koordinaten in 2D-Bildkoordinaten.
         *
         * @param aspectRatio      Das Seitenverhältnis (Breite / Höhe) des Ansichtsfensters
         * @return glm::mat4       Die Projektion-Matrix
         */
        glm::mat4 getProjectionMatrix(float aspectRatio) const;

        /**
         * @brief Aktualisiert die Position der Kamera.
         *
         * @param newPosition      Die neue Position der Kamera
         */
        void updateCameraPosition(glm::vec3 newPosition);

        /**
         * @brief Aktualisiert die Ausrichtung der Kamera.
         *
         * @param newFront         Die neue Blickrichtung der Kamera
         * @param newUp            Die neue "Oben"-Richtung der Kamera
         */
        void updateCameraOrientation(glm::vec3 newFront, glm::vec3 newUp);

        /**
         * @brief Gibt die aktuelle Position der Kamera zurück.
         *
         * @return glm::vec3       Die Position der Kamera
         */
        glm::vec3 getPosition() const;

    private:

    glm::vec3 position;  // Position der Kamera
    glm::vec3 front;     // Blickrichtung der Kamera
    glm::vec3 up;        // Obere Achse der Kamera
    float fov;           // Sichtfeld (Field of View)
    float nearClip;      // Nah-Clipping-Ebene
    float farClip;       // Fern-Clipping-Ebene

};

#endif
