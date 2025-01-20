#ifndef RAY_H
#define RAY_H

#include "RT_Vector.h"



class Ray {
    public:
        Ray();
        Ray(const Vector &origin_, const Vector &direction_);


        [[nodiscard]] Vector getRayOrigin() const;
        [[nodiscard]] Vector getRayDirection() const;

    private:
        Vector ray_origin;
        Vector ray_direction;
};

#endif //RAY_H
