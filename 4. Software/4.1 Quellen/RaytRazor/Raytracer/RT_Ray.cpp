#include "RT_Ray.h"

Ray::Ray() {
    ray_origin = Vector(0, 0, 0);
    ray_direction = Vector(1, 0, 0);
}

Ray::Ray(Vector origin_, Vector direction_) {
    ray_origin = origin_;
    ray_direction = direction_;
}

Vector Ray::getRayDirection() {
    return ray_direction;
}

Vector Ray::getRayOrigin() {
    return ray_origin;
}
