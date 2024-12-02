//
// Created by leonm on 11/28/2024.
//

#ifndef RAY_H
#define RAY_H

#include "RT_Vector.h"

// TODO: Kommentare schreiben

class Ray {
    public:
        Ray();
        Ray(const Vector<double>& origin, const Vector<double>& direction);
        Vector<double> getOrigin() const;
        Vector<double> getDirection() const;

        Vector<double> ray_origin {3};
        Vector<double> ray_direction {3};
        Vector<double> ray_ab {3};
};

#endif //RAY_H
