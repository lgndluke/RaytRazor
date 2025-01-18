#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "RT_Object.h"
#include "RT_Vector.h"
#include "RT_Color.h"

class Triangle : public Object {
    public:
        Triangle();
        Triangle(const Vector &v1_, const Vector &v2_, const Vector &v3_, const Color &color_);
        Vector getTriangleNormal();
        double getTriangleDistance();
        Color getColor() override;
        Vector getNormalAt(Vector point) override;
        double hit(Ray ray) override;

    private:
        Vector v1, v2, v3;
        Vector normal;
        double distance{};
        Color color;
};

#endif //RT_TRIANGLE_H
