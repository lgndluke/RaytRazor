#include "RT_Plane.h"

Plane::Plane() {
    normal = Vector(1, 0, 0);
    distance = 0;
    color = RT_Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane(const Vector &_normal, const double _distance, const RT_Color &_color) {
    normal = _normal;
    distance = _distance;
    color = _color;
}

Vector Plane::getPlaneNormal() const {return normal;}
double Plane::getPlaneDistance() const {return distance;}
RT_Color Plane::getColor() {return color;}

Vector Plane::getNormalAt(Vector point) {return normal;}

double Plane::hit(const Ray ray) {
    const Vector ray_direction = ray.getRayDirection();

    const double a = ray_direction.dot(normal);

    if (a == 0) {
        return -1;
    }
    const double b = normal.dot(ray.getRayOrigin().add(normal.multiply(distance).negative()));
    return -1 * b / a;
}



