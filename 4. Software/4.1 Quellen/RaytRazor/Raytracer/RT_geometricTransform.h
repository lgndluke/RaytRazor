#ifndef GEOMETRICTRANSFORM_H
#define GEOMETRICTRANSFORM_H

#include "RT_Vector.h"
#include "RT_Matrix.h"
#include "RT_Ray.h"

constexpr bool FWDTFORM = true; // Flag für Vorwärtstransformation (Lokales Koordinatensystem -> Globales Koordinatensystem)
constexpr bool BWDTFORM = false; // Flag für Rückwärtstransformation (Globales Koordinatensystem -> Lokales Koordinatensystem)


namespace RT {
    /**
    * @brief Klasse Geometric Transform ist zuständig für die Transformation / Rotation / Skalierung von Objekten.
    * @author Leon Musliu
    */
    class GeometricTransform {
    public:
        /**
        * @brief Standardkonstruktor initialisiert die Vorwärts- und Rückwartstransformationsmatrizen
        * als Einheitsmatrizen.
        */
        GeometricTransform();
        /**
        * @brief Standarddestruktor. Momentan ungenutzt.
        */
        ~GeometricTransform();
        /**
        * @brief Spezialisierter Konstruktor welcher die Vorwärts- und Rückwartstransformationsmatrizen
        * mit den angegebenen Parametern initialisiert.
        * @param fwd Vorwärtstransformationsmatrix.
        * @param bwd Rückwärtstransformationsmatrix.
        */
        GeometricTransform(const qbMatrix2<double>& fwd, const qbMatrix2<double>& bwd);
        /**
        * @brief Wendet die angegebenen Transformationen auf die FWD.- und BWD. Transformationsmatrizen an.
        * @param translation Bewegung in X, Y, Z Achse als Vektor.
        * @param rotation Rotation in X, Y, Z Achse als Vektor.
        * @param scale Skalierung in X, Y, Z Achse als Vektor.
        */
        void setTransform(const Vector<double>& translation,
                          const Vector<double>& rotation,
                          const Vector<double>& scale);
        /**
        * @brief Liefert die Vorwärtstransformationsmatrix zurück.
        * @returns Die Vorwärtstransformationsmatrix.
        */
        qbMatrix2<double> getForward();
        /**
        * @brief Liefert die Rückwärtstransformationsmatrix zurück.
        * @returns Die Rückwärtstransformationsmatrix.
        */
        qbMatrix2<double> getBackward();
        /**
        * @brief Wendet die momentane Transformationsmatrix auf einen Ray an.
        * @param inputRay Der Ray der transformiert werden soll.
        * @param dirFlag Rückwärts- oder Vorwärtstransformation?
        * @returns Der Transformierte Ray.
        */
        Ray applyTransform(const Ray& inputRay, bool dirFlag);
        /**
        * @brief Wendet die momentane Transformationsmatrix auf einen Vektor an.
        * @param inputVector Der Vektor der transformiert werden soll.
        * @param dirFlag Rückwärts- oder Vorwärtstransformation?
        * @returns Der Transformierte Vektor.
        */
        Vector<double> applyTransform(const Vector<double>& inputVector, bool dirFlag);
        /**
        * @brief Überladet den Multiplikations-Operator, um mehrere Transformationen zu kombinieren.
        * @param lhs Die linke Transformationsmatrix.
        * @param rhs Die rechte Transformationsmatrix.
        * @returns Die Kombinierte Transformationsmatrix.
        */
        friend GeometricTransform operator*(const GeometricTransform& lhs, const GeometricTransform& rhs);
        /**
        * @brief Überladet den Zuweisungs-Operator, um die rechte Transformationsmatrix in die Linke zu schieben.
        * @param rhs Die rechte Transformationsmatrix.
        * @returns Die zugewiesene Transformationsmatrix.
        */
        GeometricTransform operator=(const GeometricTransform& rhs);
        /**
        * @brief Leitet die Vorwärts- oder die Rückwärtstransformationsmatrix an die Methode Print() weiter. (Debugging)
        * @param dirFlag Rückwärts- oder Vorwärtstransformation?
        */
        void printMatrix(bool dirFlag);
        /**
        * @brief Gibt den angegebenen Vektor mit std::cout auf dem Bildschirm aus. (Debugging)
        * @param inputVector Der auszugebende Vektor.
        */
        static void printVector(const Vector<double> &inputVector);
    private:
        /**
        * @brief Gibt die angegebene Matrix mit std::cout auf dem Bildschirm aus. (Debugging)
        * @param matrix Die auszugebende Matrix.
        */
        void Print(const qbMatrix2<double> &matrix);
        qbMatrix2<double> forwardTransform {4, 4}; // (Lokales Koordinatensystem -> Globales Koordinatensystem)
        qbMatrix2<double> backwardTransform {4, 4}; // (Globales Koordinatensystem -> Lokales Koordinatensystem)
    };
}


#endif //GEOMETRICTRANSFORM_H
