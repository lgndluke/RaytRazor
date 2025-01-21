#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "RT_Object.h"
#include "RT_Vector.h"
#include "RT_Color.h"

class Triangle : public RT_Object {
    public:
        Triangle();
        Triangle(const Vector &v1_, const Vector &v2_, const Vector &v3_, const RT_Color &color_);
        Triangle(const Vector &v1_, const Vector &v2_, const Vector &v3_, const Vector &normal_, const RT_Color &color_);
        Vector getTriangleNormal();
        double getTriangleDistance();
        RT_Color getColor() override;
        Vector getNormalAt(Vector point) override;
        double hit(Ray ray) override;

    private:
        Vector v1, v2, v3;
        Vector normal;
        double distance{};
        RT_Color color;
        bool normalsSet;
};

#endif //RT_TRIANGLE_H
