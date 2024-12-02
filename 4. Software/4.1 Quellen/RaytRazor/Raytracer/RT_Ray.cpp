#include "RT_Ray.h"

Ray::Ray() {
    ray_origin = Vector<double>{std::vector<double>{0.0, 0.0, 0.0}};
    ray_direction = Vector<double>{std::vector<double>{0.0, 0.0, 1.0}};
    ray_ab = ray_direction - ray_origin;
}

Ray::Ray(const Vector<double> &origin_, const Vector<double> &direction_) {
    ray_origin = origin_;
    ray_direction = direction_;
    ray_ab = ray_direction - ray_origin;
}

Vector<double> Ray::getDirection() const {
    return ray_direction;
}

Vector<double> Ray::getOrigin() const {
    return ray_origin;
}
