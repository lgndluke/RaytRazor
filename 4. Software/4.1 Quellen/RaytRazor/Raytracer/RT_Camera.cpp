#include "RT_Camera.h"
#include "RT_Ray.h"
#include <math.h>

Camera::Camera() {
    cam_position = Vector<double>{std::vector<double> {0.0, -10.0, 0.0}};
    cam_lookAt = Vector<double>{std::vector<double> {0.0, 0.0, 0.0}};
    cam_upVector = Vector<double>{std::vector<double> {0.0, 0.0, 1.0}};
    cam_length = 1.0;
    cam_HorizontalSize = 1.0;
    cam_AspectRatio = 1.0;
}

void Camera::setPosition(const Vector<double> &newPosition) {
    cam_position = newPosition;
}

void Camera::setLookAt(const Vector<double> &newLookAt) {
    cam_lookAt = newLookAt;
}

void Camera::setUpVector(const Vector<double> &newUpVector) {
    cam_upVector = newUpVector;
}

void Camera::setLength(double newLength) {
    cam_length = newLength;
}

void Camera::setHorizontalSize(double newSize) {
    cam_HorizontalSize = newSize;
}

void Camera::setAspectRatio(double newAspectRatio) {
    cam_AspectRatio = newAspectRatio;
}

Vector<double> Camera::getPosition() {
    return cam_position;
}

Vector<double> Camera::getLookAt() {
    return cam_lookAt;
}

Vector<double> Camera::getUpVector() {
    return cam_upVector;
}

double Camera::getLength() {
    return cam_length;
}

double Camera::getHorizontalSize() {
    return cam_HorizontalSize;
}

double Camera::getAspectRatio() {
    return cam_AspectRatio;
}

Vector<double> Camera::getU() {
    return cam_projectionU;
}

Vector<double> Camera::getV() {
    return cam_projectionV;
}

Vector<double> Camera::getScreenCentre() {
    return cam_projectionCentre;
}

void Camera::update() {
    // 1. Berechnet zuerst den Vektor von der Kamera zur lookAt Position.
    cam_alignment = cam_lookAt - cam_position;
    cam_alignment.Normalize();
    // 2. Berechnet danach die U, und V Vektoren.
    cam_projectionU = Vector<double>::cross(cam_alignment, cam_upVector);
    cam_projectionU.Normalize();

    cam_projectionV = Vector<double>::cross(cam_projectionU, cam_alignment);
    cam_projectionV.Normalize();

    // 3. Danach wird die Mitte des Bildschirms berechnet.
    cam_projectionCentre = cam_position + (cam_length * cam_alignment);

    // 4. Zuletzt werden die U und V vektoren angepasst damit Sie mit der horizontal Size und dem AspectRatio übereinstimmen.
    cam_projectionU = cam_projectionU * cam_HorizontalSize;
    cam_projectionV = cam_projectionV * (cam_HorizontalSize/cam_AspectRatio);
}

bool Camera::generateRay(double x, double y, Ray & cameraRay) {
    // 1. Bildschirmpunkt in Weltkoordinaten umwandeln
    Vector<double> screenPos1 = cam_projectionCentre + (cam_projectionU * x);
    Vector<double> screenCoordinate = screenPos1 + (cam_projectionV * y);
    // 2. Ray berechnen
    cameraRay.ray_origin = cam_position;
    cameraRay.ray_direction = screenCoordinate;
    cameraRay.ray_ab = screenCoordinate - cam_position;
    return true;
}
