#ifndef PLANE_H
#define PLANE_H

#include "RT_Object.h"
#include "RT_geometricTransform.h"

// TODO: Kommentare schreiben

class Plane : public Object {
    public:
        Plane();
        virtual ~Plane() override;
        virtual bool hit(const Ray &ray, Vector<double> &point, Vector<double> &normal, Vector<double> &color) override;
    private:
};

#endif //PLANE_H
