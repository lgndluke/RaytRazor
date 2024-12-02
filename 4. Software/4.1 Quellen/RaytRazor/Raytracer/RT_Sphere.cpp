#include "RT_Sphere.h"
#include <cmath>

Sphere::Sphere() {

}

Sphere::~Sphere() {

}

bool Sphere::hit(const Ray &ray, Vector<double> &point, Vector<double> &normal, Vector<double> &color) {
    Ray local_ray = obj_transform.applyTransform(ray, BWDTFORM);

    Vector<double> vhat = local_ray.ray_ab;
    vhat.Normalize();
    double b = 2.0 * Vector<double>::dot(local_ray.ray_origin, vhat);
    double c = Vector<double>::dot(local_ray.ray_origin, local_ray.ray_origin) - 1.0;
    double intersectTest = (b * b) - 4.0 * c;
    Vector<double> poi; //Point of Intersect
    if (intersectTest > 0.0) {
        double t1 = (-b + sqrt(intersectTest)) / 2.0;
        double t2 = (-b - sqrt(intersectTest)) / 2.0;

        if (t1 < 0.0 || t2 < 0.0) return false;
        if (t1 < t2) poi = local_ray.ray_origin + (vhat * t1);
        else poi = local_ray.ray_origin + (vhat * t2);

        point = obj_transform.applyTransform(poi, FWDTFORM);
        Vector<double> objOrigin = Vector<double> {std::vector<double>{0.0, 0.0, 0.0}};
        Vector<double> newObjOrigin = obj_transform.applyTransform(objOrigin, FWDTFORM);

        normal = point - newObjOrigin;
        normal.Normalize();
        color = obj_color;

        return true;
    }
    return false;
}
