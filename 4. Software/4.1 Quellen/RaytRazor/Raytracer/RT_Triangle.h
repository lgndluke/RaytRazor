#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "RT_Object.h"
#include "RT_geometricTransform.h"

class Triangle : public Object {
    public:
        Triangle(const Vector<double>& _v0, const Vector<double>& _v1, const Vector<double>& _v2);
        Triangle(const Vector<double>& _v0, const Vector<double>& _v1, const Vector<double>& _v2,
                const Vector<double>& _n0, const Vector<double>& _n1,  const Vector<double>& _n2);
        virtual ~Triangle() override;
        virtual bool hit(const Ray &ray, Vector<double> &point, Vector<double> &normal, Vector<double> &color) override;
    private:
        Vector<double> v0, v1, v2;
        Vector<double> n0, n1, n2;
};

#endif //RT_TRIANGLE_H
