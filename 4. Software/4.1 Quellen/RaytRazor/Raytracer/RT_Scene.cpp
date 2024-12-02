#include "RT_Scene.h"

Scene::Scene() {
    camera.setPosition(Vector<double>{std::vector<double>{0.0, -10.0, -2.0}});
    camera.setLookAt(Vector<double>{std::vector<double>{0.0, 0.0, 0.0}});
    camera.setUpVector(Vector<double>{std::vector<double>{0.0, 0.0, 1.0}});
    camera.setHorizontalSize(0.25);
    camera.setAspectRatio(16.0 / 9.0);
    camera.update();

    objects.push_back(std::make_shared<Sphere>(Sphere()));
    objects.push_back(std::make_shared<Sphere>(Sphere()));
    objects.push_back(std::make_shared<Sphere>(Sphere()));

    //PLANE
    objects.push_back(std::make_shared<Plane>(Plane()));
    objects.at(3) -> obj_color = Vector<double>{std::vector<double>{0.5, 0.5, 0.5}};

    RT::GeometricTransform planeMatrix;
    planeMatrix.setTransform(Vector<double>{std::vector<double>{0.0, 0.0, 0.75}},
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{4.0, 4.0, 1.0}});

    RT::GeometricTransform testMatrix1, testMatrix2, testMatrix3;
    testMatrix1.setTransform(Vector<double>{std::vector<double>{-1.5, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.5, 0.5, 0.75}});

    testMatrix2.setTransform(Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.75, 0.5, 0.5}});

    testMatrix3.setTransform(Vector<double>{std::vector<double>{1.5, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.75, 0.75, 0.75}});

    objects.at(0) -> setTransformMatrix(testMatrix1);
    objects.at(1) -> setTransformMatrix(testMatrix2);
    objects.at(2) -> setTransformMatrix(testMatrix3);
    objects.at(3) -> setTransformMatrix(planeMatrix);

    objects.at(0) -> obj_color = Vector<double>{std::vector<double>{1.0, 0.0, 1.0}};
    objects.at(1) -> obj_color = Vector<double>{std::vector<double>{0.353, 0.0, 1.0}};
    objects.at(2) -> obj_color = Vector<double>{std::vector<double>{1.0, 0.235, 0.0}};

    lights.push_back(std::make_shared<PointLight>(PointLight()));
    lights.at(0) -> lght_location = Vector<double>{std::vector<double>{8.0, -10.0, -5.0}};
    lights.at(0) -> lght_color = Vector<double>{std::vector<double>{1.0, 1.0, 1.0}};
}

bool Scene::render(Image &output) {
    int xSize = output.getXSize();
    int ySize = output.getYSize();
    Ray cameraRay;
    Vector<double> intersectionPoint {3};
    Vector<double> localNormal {3};
    Vector<double> localColor {3};
    double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
    double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
    double minDist = 1e6;
    double maxDist = 0.0;
    for (int x = 0; x < xSize; x++) {
        for (int y = 0; y < ySize; y++) {
            double normalX = (static_cast<double>(x) * xFact) - 1.0;
            double normalY = (static_cast<double>(y) * yFact) - 1.0;

            camera.generateRay(normalX, normalY, cameraRay);

            std::shared_ptr<Object> closestObject;
            Vector<double> closestPoint {3};
            Vector<double> closestNormal {3};
            Vector<double> closestColor {3};
            double minDist = 1e6;
            bool intersectionFound = false;
            for (auto currentObj : objects) {
                bool validIntersection = currentObj -> hit(cameraRay, intersectionPoint, localNormal, localColor);
                if (validIntersection) {
                    intersectionFound = true;
                    double dist = (intersectionPoint - cameraRay.ray_origin).norm();
                    if (dist < minDist) {
                        minDist = dist;
                        closestObject = currentObj;
                        closestPoint = intersectionPoint;
                        closestNormal = localNormal;
                        closestColor = localColor;
                    }
                }
            }
            if (intersectionFound) {
                double intensity;
                Vector<double> color {3};
                double red= 0.0;
                double green= 0.0;
                double blue= 0.0;
                bool validIllumination = false;
                bool illuminated = false;
                for (auto currentLight : lights) {
                    validIllumination = currentLight -> computeIllum(closestPoint, closestNormal, objects, closestObject, color, intensity);
                    if (validIllumination) {
                        illuminated = true;
                        red += color.GetElement(0) * intensity;
                        green += color.GetElement(1) * intensity;
                        blue += color.GetElement(2) * intensity;
                    }
                }
                if (illuminated) {
                    red *= closestColor.GetElement(0);
                    green *= closestColor.GetElement(1);
                    blue *= closestColor.GetElement(2);
                    output.SetPixel(x, y, red, green, blue);
                }
            }
        }
    }
    return true;
}

