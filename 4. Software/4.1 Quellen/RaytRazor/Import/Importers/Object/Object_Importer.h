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
        static std::vector<int> fetch_indices(const boost::uuids::uuid& uuid,
                                              const string& path_to_file);

        /**
         * @brief Methode, um den Inhalt des Vektors vertices zu füllen.
         * @return std::vector<Vertex>    Den importieren Inhalt des Vektors vertices.
         */
        static vector<Vertex> fetch_vertices(const boost::uuids::uuid& uuid,
                                             const string& path_to_file);

};

#endif
