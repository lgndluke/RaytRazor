#include "RT_Plane.h"

Plane::Plane() {
    normal = Vector(1, 0, 0);
    distance = 0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane(Vector _normal, double _distance, Color _color) {
    normal = _normal;
    distance = _distance;
    color = _color;
}

Vector Plane::getPlaneNormal() {return normal;}
double Plane::getPlaneDistance() {return distance;}
Color Plane::getColor() {return color;}

Vector Plane::getNormalAt(Vector point) {return normal;}

double Plane::hit(Ray ray) {
    Vector ray_direction = ray.getRayDirection();

    double a = ray_direction.dot(normal);

    if (a == 0) {
        return -1;
    }
    double b = normal.dot(ray.getRayOrigin().add(normal.multiply(distance).negative()));
    return -1 * b / a;
}



