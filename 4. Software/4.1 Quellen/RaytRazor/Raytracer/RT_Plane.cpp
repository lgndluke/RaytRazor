#include "RT_Plane.h"
#include <cmath>

Plane::Plane() {

}

Plane::~Plane() {

}

bool Plane::hit(const Ray &ray, Vector<double> &point, Vector<double> &normal, Vector<double> &color) {
    Ray local_ray = obj_transform.applyTransform(ray, BWDTFORM);
    Vector<double> k = local_ray.ray_ab;
    k.Normalize();

    if (!closeEnough(k.GetElement(2), 0.0)) {
        double t = local_ray.ray_origin.GetElement(2) / -k.GetElement(2);
        if (t > 0.0) {
            double u = local_ray.ray_direction.GetElement(0) + (k.GetElement(0) * t);
            double v = local_ray.ray_direction.GetElement(1) + (k.GetElement(1) * t);

            if (abs(u) < 1.0 && abs(v) < 1.0) {
                Vector<double> poi = local_ray.ray_origin + t * k;
                point = obj_transform.applyTransform(poi, FWDTFORM);
                Vector<double> localOrigin {std::vector<double>{0.0, 0.0, 0.0}};
                Vector<double> normalVector {std::vector<double>{0.0, 0.0, -1.0}};
                Vector<double> globalOrigin = obj_transform.applyTransform(localOrigin, FWDTFORM);
                normal = obj_transform.applyTransform(normalVector, FWDTFORM) - globalOrigin;
                normal.Normalize();
                color = obj_color;
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}
