#include"RT_geometricTransform.h"

RT::GeometricTransform::GeometricTransform() {
    forwardTransform.SetToIdentity();
    backwardTransform.SetToIdentity();
}

RT::GeometricTransform::~GeometricTransform() {

}

RT::GeometricTransform::GeometricTransform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bwd) {
    if (fwd.GetNumRows() != 4 || fwd.GetNumCols() != 4 || bwd.GetNumRows() != 4 || bwd.GetNumCols() != 4) {
        throw std::invalid_argument("False inputs!");
    }
    forwardTransform = fwd;
    backwardTransform = bwd;
}

void RT::GeometricTransform::setTransform(const Vector<double> &translation, const Vector<double> &rotation, const Vector<double> &scale) {
    qbMatrix2<double> translationMatrix {4, 4};
    qbMatrix2<double> rotationMatrixX {4, 4};
    qbMatrix2<double> rotationMatrixY {4, 4};
    qbMatrix2<double> rotationMatrixZ {4, 4};
    qbMatrix2<double> scaleMatrix {4, 4};

    translationMatrix.SetToIdentity();
    rotationMatrixX.SetToIdentity();
    rotationMatrixY.SetToIdentity();
    rotationMatrixZ.SetToIdentity();
    scaleMatrix.SetToIdentity();

    translationMatrix.SetElement(0, 3, translation.GetElement(0));
    translationMatrix.SetElement(1, 3, translation.GetElement(1));
    translationMatrix.SetElement(2, 3, translation.GetElement(2));

    rotationMatrixZ.SetElement(0, 0, cos(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(0, 1, -sin(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(1, 0, sin(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(1, 1, cos(rotation.GetElement(2)));

    rotationMatrixY.SetElement(0, 0, cos(rotation.GetElement(1)));
    rotationMatrixY.SetElement(0, 2, sin(rotation.GetElement(1)));
    rotationMatrixY.SetElement(2, 0, -sin(rotation.GetElement(1)));
    rotationMatrixY.SetElement(2, 2, cos(rotation.GetElement(1)));

    rotationMatrixX.SetElement(1, 1, cos(rotation.GetElement(0)));
    rotationMatrixX.SetElement(1, 2, -sin(rotation.GetElement(0)));
    rotationMatrixX.SetElement(2, 1, sin(rotation.GetElement(0)));
    rotationMatrixX.SetElement(2, 2, cos(rotation.GetElement(0)));

    scaleMatrix.SetElement(0, 0, scale.GetElement(0));
    scaleMatrix.SetElement(1, 1, scale.GetElement(1));
    scaleMatrix.SetElement(2, 2, scale.GetElement(2));

    forwardTransform = translationMatrix * scaleMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ;

    backwardTransform = forwardTransform;
    backwardTransform.Inverse();
}

qbMatrix2<double> RT::GeometricTransform::getForward() {
    return forwardTransform;
}

qbMatrix2<double> RT::GeometricTransform::getBackward() {
    return backwardTransform;
}

Ray RT::GeometricTransform::applyTransform(const Ray &ray, bool dirFlag) {
    Ray result;
    if (dirFlag) {
        result.ray_origin = this->applyTransform(ray.ray_origin, FWDTFORM);
        result.ray_direction = this->applyTransform(ray.ray_direction, FWDTFORM);
        result.ray_ab = result.ray_direction - result.ray_origin;
    }
    else {
        result.ray_origin = this->applyTransform(ray.ray_origin, BWDTFORM);
        result.ray_direction = this->applyTransform(ray.ray_direction, BWDTFORM);
        result.ray_ab = result.ray_direction - result.ray_origin;
    }

    return result;
}

Vector<double> RT::GeometricTransform::applyTransform(const Vector<double> &vector, bool dirFlag) {
    std::vector<double> tmpData {vector.GetElement(0), vector.GetElement(1), vector.GetElement(2), 1.0};
    Vector<double> tmpVec {tmpData};
    Vector<double> result;

    if (dirFlag) {
        result = forwardTransform * tmpVec;
    }
    else {
        result = backwardTransform * tmpVec;
    }
    Vector<double> outputVector{std::vector<double> {result.GetElement(0), result.GetElement(1), result.GetElement(2)}};
    return outputVector;
}

namespace RT {
    RT::GeometricTransform operator*(const RT::GeometricTransform &lhs, const RT::GeometricTransform &rhs) {
        qbMatrix2<double> fwdResult = lhs.forwardTransform * rhs.forwardTransform;
        qbMatrix2<double> bwdResult = fwdResult;
        bwdResult.Inverse();

        RT::GeometricTransform result (fwdResult, bwdResult);
        return result;
    }
}

    RT::GeometricTransform RT::GeometricTransform::operator=(const GeometricTransform &rhs) {
        if (this != &rhs) {
            forwardTransform = rhs.forwardTransform;
            backwardTransform = rhs.backwardTransform;
        }
        return *this;
    }

    void RT::GeometricTransform::printMatrix(bool dirFlag) {
        if (dirFlag) {
            Print(forwardTransform);
        }
        else {
            Print(backwardTransform);
        }
    }

    void RT::GeometricTransform::Print(const qbMatrix2<double> &matrix) {
        int nRows = matrix.GetNumRows();
        int nCols = matrix.GetNumCols();
        for (int row = 0; row < nRows; row++) {
            for (int col = 0; col < nCols; col++) {
                std::cout << std::fixed << matrix.GetElement(row, col) << " ";
            }
            std::cout << std::endl;
        }
    }

    void RT::GeometricTransform::printVector(const Vector<double> &vector) {
        int nRows = vector.GetNumDims();
        for (int row = 0; row < nRows; row++) {
            std::cout << std::fixed << vector.GetElement(row) << " " << std::endl;
        }
    }


