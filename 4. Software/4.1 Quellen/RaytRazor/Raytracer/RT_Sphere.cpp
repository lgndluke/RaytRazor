#include "RT_Sphere.h"

Sphere::Sphere() {
    center = Vector(0, 0, 0);
    radius = 1.0;
    color = Color(0.5, 0.5, 0.5, 0);
}
Sphere::Sphere(const Vector &_center, const double _radius, const Color &_color) {
    center = _center;
    radius = _radius;
    color = _color;
}

Vector Sphere::getCenter() const {return center;}
double Sphere::getRadius() const {return radius;}
Color Sphere::getColor() {return color;}

Vector Sphere::getNormalAt(Vector point) {
    const Vector normal_Vect = point.add(center.negative()).normalize();
    return normal_Vect;
}

double Sphere::hit(const Ray ray) {
    const Vector ray_origin = ray.getRayOrigin();
    const double ray_origin_x = ray_origin.getX();
    const double ray_origin_y = ray_origin.getY();
    const double ray_origin_z = ray_origin.getZ();

    const Vector ray_direction = ray.getRayDirection();
    const double ray_direction_x = ray_direction.getX();
    const double ray_direction_y = ray_direction.getY();
    const double ray_direction_z = ray_direction.getZ();

    const Vector sphere_center = center;
    const double sphere_center_x = sphere_center.getX();
    const double sphere_center_y = sphere_center.getY();
    const double sphere_center_z = sphere_center.getZ();

    const double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) + (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) + (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
    const double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);

    if (const double discriminant = b * b - 4 * c; discriminant > 0) {

        if (double root_1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001; root_1 > 0) {
            return root_1;
        }
        double root_2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
        return root_2;
    }
        return -1;
}
