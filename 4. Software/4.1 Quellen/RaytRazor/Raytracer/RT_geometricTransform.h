#ifndef GEOMETRICTRANSFORM_H
#define GEOMETRICTRANSFORM_H

#include "RT_Vector.h"
#include "RT_Matrix.h"
#include "RT_Ray.h"

constexpr bool FWDTFORM = true;
constexpr bool BWDTFORM = false;

// TODO: Kommentare schreiben

namespace RT {
    class GeometricTransform {
    public:
        GeometricTransform();
        ~GeometricTransform();
        GeometricTransform(const qbMatrix2<double>& fwd, const qbMatrix2<double>& bwd);

        void setTransform(const Vector<double>& translation,
                          const Vector<double>& rotation,
                          const Vector<double>& scale);

        qbMatrix2<double> getForward();
        qbMatrix2<double> getBackward();

        Ray applyTransform(const Ray& inputRay, bool dirFlag);
        Vector<double> applyTransform(const Vector<double>& inputVector, bool dirFlag);

        friend GeometricTransform operator*(const GeometricTransform& lhs, const GeometricTransform& rhs);
        GeometricTransform operator=(const GeometricTransform& rhs);
        void printMatrix(bool dirFlag);
        static void printVector(const Vector<double> &inputVector);
    private:
        void Print(const qbMatrix2<double> &matrix);
        qbMatrix2<double> forwardTransform {4, 4};
        qbMatrix2<double> backwardTransform {4, 4};
    };
}


#endif //GEOMETRICTRANSFORM_H
