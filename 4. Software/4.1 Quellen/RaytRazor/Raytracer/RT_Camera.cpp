#include "RT_Camera.h"

Camera::Camera() {
    campos = Vector(0, 0, 0);
    camdir = Vector(0, 0, 1);
    camright = Vector(0, 0, 0);
    camdown = Vector(0, 0, 0);
}

Camera::Camera(const Vector &_campos, const Vector &_camdir, const Vector &_camright, const Vector &_camdown) {
    campos = _campos;
    camdir = _camdir;
    camright = _camright;
    camdown = _camdown;
}




Vector Camera::getCamPos() const { return campos; }
Vector Camera::getCamDir() const { return camdir; }
Vector Camera::getCamRight() const { return camright; }
Vector Camera::getCamDown() const { return camdown; }



