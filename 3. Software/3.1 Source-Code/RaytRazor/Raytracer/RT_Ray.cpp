#include "RT_Ray.h"

Ray::Ray() {
    ray_origin = Vector(0, 0, 0);
    ray_direction = Vector(1, 0, 0);
}

Ray::Ray(const Vector &origin_, const Vector &direction_) {
    ray_origin = origin_;
    ray_direction = direction_;
}

Vector Ray::getRayDirection() const {
    return ray_direction;
}

Vector Ray::getRayOrigin() const {
    return ray_origin;
}
