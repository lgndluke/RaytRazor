#include "RT_Light.h"

Light::Light() {
    position = Vector(0, 0, 0);
    color = Color(1, 1, 1, 0);
}
Light::Light(const Vector &_position, const Color &_color) {
    position = _position;
    color = _color;
}

Vector Light::getLightPosition() { return position; }
Color Light::getLightColor() { return color; }



