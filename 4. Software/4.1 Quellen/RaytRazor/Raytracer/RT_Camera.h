//
// Created by leonm on 11/28/2024.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "RT_Vector.h"
#include "RT_Ray.h"

/**
 * @brief Klasse RayTrace_Camera ist zuständig für die Kamera, welche die Szene sieht.
 * @author Leon Musliu
 */
class Camera {
    public:
    /** @brief Der Default Konstruktor initialisiert eine Standardkamera. */
    Camera();
    /**
     * @brief Ändert die aktuelle Position der Kamera im Raum.
     * @param newPosition Vektor<double> welcher die XYZ Koordinaten der neuen Position enthält.
     */
    void setPosition(const Vector<double> & newPosition);
    /**
     * @brief Ändert die Blickrichtung der Kamera im Raum.
     * @param newLookAt Vektor<double> welcher die XYZ Koordinaten der neuen Position enthält.
     */
    void setLookAt(const Vector<double> & newLookAt);
    /**
     * @brief Ändert den Orientierungsvektor der Kamera im Raum. Er wird genutzt damit die Kamera nicht auf dem Kopf steht.
     * @param newUpVector Vektor<double> welcher die XYZ Koordinaten der neuen Position enthält.
     */
    void setUpVector(const Vector<double> & newUpVector);
    /**
     * @brief Ändert die Brennweite der Kamera im Raum.
     * Großer Wert = Reinzoomen.
     * @param newLength Double enthält die neue Brennweite.
     */
    void setLength(double newLength);
    /**
     * @brief Ändert die physische Breite des Bildfensters im Raum.
     * Großer Wert = Herauszoomen. Standard: 0.25
     * @param newSize Double enthält die neue FOV.
     */
    void setHorizontalSize(double newSize);
    /**
     * @brief Ändert das Verhältnis zwischen Höhe und Breite des Bildfensters im Raum.
     * @param newAspectRatio Double enthält das neue AspectRatio.
     */
    void setAspectRatio(double newAspectRatio);

    /**
     * @brief Liefert die momentane Position der Kamera.
     * @return Vector<double> von XYZ Koordinaten.
     */
    Vector<double> getPosition();
    /**
     * @brief Liefert die momentane Blickrichtung der Kamera.
     * @return Vector<double> von XYZ Koordinaten.
     */
    Vector<double> getLookAt();
    /**
     * @brief Liefert dem momentanen Oben-Vektor der Kamera, welcher dazu dient Oben zu definieren.
     * @return Vector<double> von XYZ Koordinaten.
     */
    Vector<double> getUpVector();
    /**
     * @brief Liefert dem momentanen U-Vektor der Kamera, welcher dazu dient die horizontale
     * Richtung des Fensters zu definieren. (Essenziell fürs Raytracing)
     * @return Vector<double> von XYZ Koordinaten.
     */
    Vector<double> getU();
    /**
     * @brief Liefert dem momentanen V-Vektor der Kamera, welcher dazu dient die vertikale
     * Richtung des Fensters zu definieren. (Essenziell fürs Raytracing)
     * @return Vector<double> von XYZ Koordinaten.
     */
    Vector<double> getV();
    /**
     * @brief Liefert dem momentanen Center-Vektor der Kamera, welcher dazu dient den Ursprung
     * der geschossenen Strahlen zu definieren. (Essenziell fürs Raytracing)
     * @return Vector<double> von XYZ Koordinaten.
     */
    Vector<double> getScreenCentre();
    /**
     * @brief Liefert die aktuelle Brennweite der Kamera.
     * @return Double der aktuellen Brennweite.
     */
    double getLength();
    /**
     * @brief Liefert die aktuelle physische Breite des Bildfensters.
     * @return Double der aktuellen Breite.
     */
    double getHorizontalSize();
    /**
     * @brief Liefert das aktuelle das Verhältnis zwischen Höhe und Breite des Bildfensters.
     * @return Double des aktuellen AspectRatios.
     */
    double getAspectRatio();
    /**
     * @brief Generiert einen Ray, indem die Position und die Richtung des Rays festgelegt werden.
     * @param x X-Koordinate
     * @param y Y-Koordinate
     * @param cameraRay Der Ray der modifiziert werden soll.
     * @return Boolean; ob es erfolgreich war.
     */
    bool generateRay(double x, double y, Ray & cameraRay);
    /**
     * @brief Berechnet die Kamerageometrie. Kommentare sind in der Funktion für nähere Funktionsweise
     */
    void update();

    private:
    Vector<double> cam_position {3};
    Vector<double> cam_lookAt {3};
    Vector<double> cam_upVector {3};
    double cam_length;
    double cam_HorizontalSize;
    double cam_AspectRatio;

    Vector<double> cam_alignment {3};
    Vector<double> cam_projectionU {3};
    Vector<double> cam_projectionV {3};
    Vector<double> cam_projectionCentre {3};
};
#endif //CAMERA_H
