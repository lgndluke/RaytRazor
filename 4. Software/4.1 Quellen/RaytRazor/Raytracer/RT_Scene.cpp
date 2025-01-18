#include "RT_Scene.h"
#include "RT_Triangle.h"
#include "../Import/Importers/Object/Object_Importer.h"
#include "../Import/Resources/Object/Object_Resource.h"

Scene::Scene() {
    //cubeScene();
    sphereScene();
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
    #pragma omp parallel for collapse(2) private(cameraRay, intersectionPoint, localNormal, localColor)
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
    Logger::log(MessageType::DEBUG, "DONE!");
    return true;
}

void Scene::cubeScene() {
    camera.setPosition(Vector<double>{std::vector<double>{5.0, -15.0, -2.0}});
    camera.setLookAt(Vector<double>{std::vector<double>{0.0, 0.0, 0.0}});
    camera.setUpVector(Vector<double>{std::vector<double>{0.0, 0.0, 1.0}});
    camera.setHorizontalSize(0.25);
    camera.setAspectRatio(16.0 / 9.0);
    camera.update();


    RT::GeometricTransform triangleMatrix;
    triangleMatrix.setTransform(Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{1.0, 1.0, 1.0}});

    std::vector<Vertex> vertices;
    try
    {
        // TODO HIER WENN MÖGLICH SPÄTER AUF RELATIVEN PFAD ANPASSEN.
        vertices = Object_Importer::fetch_vertices("C:/Uni/S5/SOPR/RaytRazor/5. Modelle/5.1 Beispielmodelle/Test/cube.obj");
    }
    catch (const std::exception &e)
    {
        Logger::log(MessageType::SEVERE, "RT_Scene::cubeScene: File not found exception\n" + std::string(e.what()));
    }

    for(int i = 0; i <= vertices.size()-3 ; i = i+3) {

        objects.push_back(std::make_shared<Triangle>(Triangle(Vector<double>{std::vector<double>{static_cast<double>(vertices[i].position.x), static_cast<double>(vertices[i].position.y), static_cast<double>(vertices[i].position.z)}},
                                                Vector<double>{std::vector<double>{static_cast<double>(vertices[(i+1)].position.x), static_cast<double>(vertices[(i+1)].position.y), static_cast<double>(vertices[(i+1)].position.z)}},
                                                Vector<double>{std::vector<double>{static_cast<double>(vertices[(i+2)].position.x), static_cast<double>(vertices[(i+2)].position.y), static_cast<double>(vertices[(i+2)].position.z)}},
                                                Vector<double>{std::vector<double>{static_cast<double>(vertices[i].normal.x), static_cast<double>(vertices[i].normal.y), static_cast<double>(vertices[i].normal.z)}},
                                                Vector<double>{std::vector<double>{static_cast<double>(vertices[(i+1)].normal.x), static_cast<double>(vertices[(i+1)].normal.y), static_cast<double>(vertices[(i+1)].normal.z)}},
                                                Vector<double>{std::vector<double>{static_cast<double>(vertices[(i+2)].normal.x), static_cast<double>(vertices[(i+2)].normal.y), static_cast<double>(vertices[(i+2)].normal.z)}})));

    }
    for (int i = 0; i < objects.size() ; i++) {
        objects.at(i) -> obj_color = Vector<double>{std::vector<double>{1.0, 0.0, 0.0}};
        objects.at(i)->setTransformMatrix(triangleMatrix);
    }

    Logger::log(MessageType::DEBUG, "Objects: " + std::to_string(objects.size()));

    objects.push_back(std::make_shared<Plane>(Plane()));
    objects.at(objects.size()-1) -> obj_color = Vector<double>{std::vector<double>{0.5, 0.5, 0.5}};

    RT::GeometricTransform planeMatrix;
    planeMatrix.setTransform(Vector<double>{std::vector<double>{0.0, 0.0, 0.6}},
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{8.0, 8.0, 2.0}});
    objects.at(objects.size()-1) -> setTransformMatrix(planeMatrix);

    lights.push_back(std::make_shared<PointLight>(PointLight()));
    lights.at(0) -> lght_location = Vector<double>{std::vector<double>{8.0, -15.0, -5.0}};
    lights.at(0) -> lght_color = Vector<double>{std::vector<double>{1.0, 1.0, 1.0}};
}

void Scene::sphereScene() {
    camera.setPosition(Vector<double>{std::vector<double>{0.0, -11.0, -2.0}});
    camera.setLookAt(Vector<double>{std::vector<double>{0.0, 0.0, 0.0}});
    camera.setUpVector(Vector<double>{std::vector<double>{0.0, 0.0, 1.0}});
    camera.setHorizontalSize(0.25);
    camera.setAspectRatio(16.0 / 9.0);
    camera.update();

    // ADDING SPHERES
    objects.push_back(std::make_shared<Sphere>(Sphere()));
    objects.push_back(std::make_shared<Sphere>(Sphere()));
    objects.push_back(std::make_shared<Sphere>(Sphere()));
    objects.push_back(std::make_shared<Sphere>(Sphere()));
    objects.push_back(std::make_shared<Sphere>(Sphere()));

    RT::GeometricTransform sphereMatrix1, sphereMatrix2, sphereMatrix3, sphereMatrix4, sphereMatrix5;
    sphereMatrix1.setTransform(Vector<double>{std::vector<double>{-1.0, 0.76, 0.25}}, // GREEN SPHERE
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.5, 0.5, 0.5}});

    sphereMatrix2.setTransform(Vector<double>{std::vector<double>{0.75, -0.5, 0.25}}, // ORANGE SPHERE
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.5, 0.5, 0.5}});

    sphereMatrix3.setTransform(Vector<double>{std::vector<double>{0.25, 3.75, 0.45}}, // RED SPHERE
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.25, 0.25, 0.25}});

    sphereMatrix4.setTransform(Vector<double>{std::vector<double>{-0.5, -2.0, 0.4}}, // BLUE SPHERE
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.4, 0.4, 0.4}});

    sphereMatrix5.setTransform(Vector<double>{std::vector<double>{-0.35, 4.5, 0.55}}, // PINK SPHERE
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{0.15, 0.15, 0.15}});

    objects.at(0) -> setTransformMatrix(sphereMatrix1);
    objects.at(1) -> setTransformMatrix(sphereMatrix2);
    objects.at(2) -> setTransformMatrix(sphereMatrix3);
    objects.at(3) -> setTransformMatrix(sphereMatrix4);
    objects.at(4) -> setTransformMatrix(sphereMatrix5);

    objects.at(0) -> obj_color = Vector<double>{std::vector<double>{0.5, 1.0, 0.5}};
    objects.at(1) -> obj_color = Vector<double>{std::vector<double>{1.0, 0.5, 0.5}};
    objects.at(2) -> obj_color = Vector<double>{std::vector<double>{1.0, 0.1, 0.1}};
    objects.at(3) -> obj_color = Vector<double>{std::vector<double>{0.1, 0.5, 1.0}};
    objects.at(4) -> obj_color = Vector<double>{std::vector<double>{0.9, 0.35, 1.0}};


    //ADDING PLANES
    objects.push_back(std::make_shared<Plane>(Plane()));
    objects.push_back(std::make_shared<Plane>(Plane()));
    objects.push_back(std::make_shared<Plane>(Plane()));
    objects.push_back(std::make_shared<Plane>(Plane()));


    RT::GeometricTransform planeMatrixBot, planeMatrixLeft, planeMatrixRight, planeMatrixBehind;
    planeMatrixBot.setTransform(Vector<double>{std::vector<double>{0.0, 0.0, 0.75}}, // BOTTOM WALL
                             Vector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             Vector<double>{std::vector<double>{8.0, 8.0, 2.0}});
    planeMatrixLeft.setTransform(Vector<double>{std::vector<double>{-2.0, 0.0, 0.75}}, // LEFT WALL
                             Vector<double>{std::vector<double>{0.0, 4.7, 0.0}},
                             Vector<double>{std::vector<double>{2.0, 8.0, 4.0}});
    planeMatrixRight.setTransform(Vector<double>{std::vector<double>{1.8, 0.0, 0.75}}, // RIGHT WALL
                             Vector<double>{std::vector<double>{0.0, -4.7, 0.0}},
                             Vector<double>{std::vector<double>{2.0, 8.0, 4.0}});
    planeMatrixBehind.setTransform(Vector<double>{std::vector<double>{-0.5, 6.75, 0.0}}, // BEHIND WALL
                             Vector<double>{std::vector<double>{4.65, 0.0, 0.1}},
                             Vector<double>{std::vector<double>{4.0, 8.0, 4.0}});
    objects.at(5) -> setTransformMatrix(planeMatrixBot);
    objects.at(6) -> setTransformMatrix(planeMatrixLeft);
    objects.at(7) -> setTransformMatrix(planeMatrixRight);
    objects.at(8) -> setTransformMatrix(planeMatrixBehind);

    objects.at(5) -> obj_color = Vector<double>{std::vector<double>{0.5, 0.5, 0.5}};
    objects.at(6) -> obj_color = Vector<double>{std::vector<double>{1.0, 0.6, 0.16}};
    objects.at(7) -> obj_color = Vector<double>{std::vector<double>{0.16, 0.6, 1.0}};
    objects.at(8) -> obj_color = Vector<double>{std::vector<double>{0.25, 0.25, 0.25}};

    // LIGHTS
    lights.push_back(std::make_shared<PointLight>(PointLight()));
    lights.at(0) -> lght_location = Vector<double>{std::vector<double>{0.0, 0.0, -2.75}};
    lights.at(0) -> lght_color = Vector<double>{std::vector<double>{1.0, 1.0, 1.0}};
}


