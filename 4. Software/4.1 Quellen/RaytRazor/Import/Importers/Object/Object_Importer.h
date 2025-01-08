#ifndef OBJECT_IMPORTER_H
#define OBJECT_IMPORTER_H

#include "../../Resources/Object/Object_Resource.h"
#include "../../Logger/Logger.h"
#include <optional>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Die Klasse Object_Importer erstellt @link Object_Resource Objekte.
 * @author Christian Kasper, Lukas Jeckle
 */
class Object_Importer
{

    public:

        /**
         * @brief Statische Klasse ohne Konstruktor.
         */
        Object_Importer() = delete;

        /**
         * @brief Methode, um den Inhalt einer .obj Datei in eine @link Object_Resource umzuwandeln.
         *
         * @param uuid                      Die UUID der @link Object_Resource
         * @param path_to_file              Der Pfad der @link Object_Resource
         *
         * @return Object_Resource        Die importierte @link Object_Resource, falls diese erstellt wurde. Andernfalls, std::nullopt.
         */
        static std::optional<Object_Resource> import_Object(const boost::uuids::uuid& uuid,
                                                            const string& path_to_file);

    private:


        /**
         * @brief Methode, um den Inhalt des Vektors indices zu füllen.
         * @return std::vector<int>         Den importierten Inhalt des Vektors indices.
         */
        static vector<Indice> fetch_indices(const string& path_to_file);

        /**
         * @brief Methode, um den Inhalt des Vektors vertices zu füllen.
         * @return std::vector<Vertex>    Den importieren Inhalt des Vektors vertices.
         */
        static vector<Vertex> fetch_vertices(const string& path_to_file);

        /**
         * @brief Methode, um die Dateiendung zu validieren.
         * @param path_to_file              Der zu überprüfende Dateiname.
         * @param suffix                    Die Dateiendung, auf die geprüft werden soll.
         * @return bool                     True, falls die Datei auf .obj endet. Andernfalls, False.
         */
        static bool validate_extension(const std::string& path_to_file, const std::string& suffix);

};

#endif

/*
Import.h

#ifndef OBJECT_IMPORTER_H
#define OBJECT_IMPORTER_H

#include <Eigen/Dense>
#include <nanogui/nanogui.h>
#include <string>

class ObjectImporter {
private:
// Private Member zur Speicherung der Daten
nanogui::MatrixXf m_vertices;
nanogui::MatrixXu m_indices;
nanogui::MatrixXf m_uvs;
nanogui::MatrixXf m_normals;
nanogui::MatrixXf m_colors;

public:
// Getter
const nanogui::MatrixXf& getVertices() const { return m_vertices; }
const nanogui::MatrixXu& getIndices() const { return m_indices; }
const nanogui::MatrixXf& getUVs() const { return m_uvs; }
const nanogui::MatrixXf& getNormals() const { return m_normals; }
const nanogui::MatrixXf& getColors() const { return m_colors; }

// OBJ-Datei laden
bool loadOBJ(const std::string& path);
};

#endif // OBJECT_IMPORTER_H
*/