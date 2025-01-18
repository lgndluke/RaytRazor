#ifndef RAY_H
#define RAY_H

#include "RT_Vector.h"



class Ray {
    public:
        Ray();
        Ray(Vector origin_, Vector direction_);


        Vector getRayOrigin();
        Vector getRayDirection();

    private:
        Vector ray_origin;
        Vector ray_direction;
};

#endif //RAY_H
