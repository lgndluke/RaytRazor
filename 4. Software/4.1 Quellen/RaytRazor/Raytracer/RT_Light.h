#ifndef LIGHT_H
#define LIGHT_H

#include <memory>
#include "RT_Vector.h"
#include "RT_Ray.h"
#include "RT_Object.h"

// TODO: Kommentare schreiben

class Light {
    public:
    Light();
    virtual ~Light();

    virtual bool computeIllum (const Vector<double> &intersect, const Vector<double> &normal,
                               const std::vector<std::shared_ptr<Object>> &objects,
                               const std::shared_ptr<Object> &object,
                               Vector<double> &color, double &intensity);

    Vector<double> lght_color {3};
    Vector<double> lght_location {3};
    double lght_intensity;
};


#endif //LIGHT_H
