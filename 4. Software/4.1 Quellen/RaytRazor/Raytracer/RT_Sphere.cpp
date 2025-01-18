#include "RT_Sphere.h"

Sphere::Sphere() {
    center = Vector(0, 0, 0);
    radius = 1.0;
    color = Color(0.5, 0.5, 0.5, 0);
}
Sphere::Sphere(Vector _center, double _radius, Color _color) {
    center = _center;
    radius = _radius;
    color = _color;
}

Vector Sphere::getCenter() {return center;}
double Sphere::getRadius() {return radius;}
Color Sphere::getColor() {return color;}

Vector Sphere::getNormalAt(Vector point) {
    Vector normal_Vect = point.add(center.negative()).normalize();
    return normal_Vect;
}

double Sphere::hit(Ray ray) {
    Vector ray_origin = ray.getRayOrigin();
    double ray_origin_x = ray_origin.getX();
    double ray_origin_y = ray_origin.getY();
    double ray_origin_z = ray_origin.getZ();

    Vector ray_direction = ray.getRayDirection();
    double ray_direction_x = ray_direction.getX();
    double ray_direction_y = ray_direction.getY();
    double ray_direction_z = ray_direction.getZ();

    Vector sphere_center = center;
    double sphere_center_x = sphere_center.getX();
    double sphere_center_y = sphere_center.getY();
    double sphere_center_z = sphere_center.getZ();

    double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) + (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) + (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
    double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);

    double discriminant = b * b - 4 * c;

    if (discriminant > 0) {
        /// the ray intersects the sphere
        double root_1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;

        if (root_1 > 0) {
            return root_1;
        }
        double root_2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
        return root_2;
    }
    else {
        // the ray missed the sphere
        return -1;
    }
}
