#ifndef OBJECT_H
#define OBJECT_H

#include "RT_Vector.h"
#include "RT_Ray.h"
#include "RT_Color.h"

// TODO: Kommentare schreiben

class RT_Object {
    public:
    virtual ~RT_Object() = default;

    RT_Object();
    virtual RT_Color getColor();
    virtual Vector getNormalAt(Vector point);
    virtual double hit(Ray ray);
};


#endif //OBJECT_H
