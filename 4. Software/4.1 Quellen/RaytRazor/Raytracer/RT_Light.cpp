#include "RT_Light.h"

Light::Light() {
    position = Vector(0, 0, 0);
    color = RT_Color(1, 1, 1, 0);
}
Light::Light(const Vector &_position, const RT_Color &_color) {
    position = _position;
    color = _color;
}

Vector Light::getLightPosition() { return position; }
RT_Color Light::getLightColor() { return color; }



