#ifndef OBJECT_H
#define OBJECT_H

#include "RT_Vector.h"
#include "RT_Ray.h"
#include "RT_geometricTransform.h"

// TODO: Kommentare schreiben

class Object {
    public:
    Object();
    virtual ~Object();
    virtual bool hit(const Ray& ray, Vector<double> &point, Vector<double> &normal, Vector<double> &color);
    void setTransformMatrix(const RT::GeometricTransform& transform);
    bool closeEnough(const double f1, const double f2);

    Vector<double> obj_color {3};
    RT::GeometricTransform obj_transform;
};


#endif //OBJECT_H
