#include "RT_Camera.h"

Camera::Camera() {
    campos = Vector(0, 0, 0);
    camdir = Vector(0, 0, 1);
    camright = Vector(0, 0, 0);
    camdown = Vector(0, 0, 0);
}

Camera::Camera(Vector _campos, Vector _camdir, Vector _camright, Vector _camdown) {
    campos = _campos;
    camdir = _camdir;
    camright = _camright;
    camdown = _camdown;
}




Vector Camera::getCamPos() { return campos; }
Vector Camera::getCamDir() { return camdir; }
Vector Camera::getCamRight() { return camright; }
Vector Camera::getCamDown() { return camdown; }



