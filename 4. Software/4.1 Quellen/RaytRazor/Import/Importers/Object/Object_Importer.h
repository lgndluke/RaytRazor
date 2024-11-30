#ifndef OBJECT_IMPORTER_H
#define OBJECT_IMPORTER_H

//TODO Add imports here.
#include "../../Resources/Object/Object_Resource.h"
#include "../../Logger/Logger.h"
#include <string>
#include <vector>
#include <Eigen/Dense>  // Für die Verwendung von Eigen-Matrizen und -Vektoren

// Die neue Funktionsdeklaration mit den Eigen Matrizen für Vertices, Indices und Farben
bool loadOBJ(
    const char* path,
    nanogui::MatrixXf& out_vertices,   // Matrix für Vertices des Modells
    nanogui::MatrixXu& out_indices,    // Matrix für Indices, um das Mesh zusammenzusetzen
    nanogui::MatrixXf& out_uvs,        // Matrix für UV-Koordinaten des Modells
    nanogui::MatrixXf& out_normals,    // Matrix für Normale des Modells
    nanogui::MatrixXf& out_colors      // Matrix für Farben der Vertices
);

#endif // OBJECT_IMPORTER_H
