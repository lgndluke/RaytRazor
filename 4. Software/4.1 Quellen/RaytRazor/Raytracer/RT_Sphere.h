#ifndef SPHERE_H
#define SPHERE_H

#include "RT_Object.h"
#include "RT_geometricTransform.h"

// TODO: Kommentare schreiben

class Sphere : public Object {
    public:
    Sphere();
    virtual ~Sphere() override;

    virtual bool hit(const Ray &ray, Vector<double> &point, Vector<double> &normal, Vector<double> &color) override;

    private:
};


#endif //SPHERE_H
