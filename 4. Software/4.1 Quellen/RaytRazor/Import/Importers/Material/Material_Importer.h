#ifndef MATERIAL_IMPORTER_H
#define MATERIAL_IMPORTER_H

#include "../../Resources/Material/Material_Resource.h"
#include "../../Logger/Logger.h"
#include <optional>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Die Klasse Material_Importer erstellt @link Material_Resource Objekte.
 * @author Christian Kasper, Lukas Jeckle
 */
class Material_Importer
{

    public:

        /**
         * @brief Statische Klasse ohne Konstruktor.
         */
        Material_Importer() = delete;

        /**
         * @brief Methode, um den Inhalt einer .mtl Datei in eine @link Material_Resource umzuwandeln.
         *
         * @param uuid                      Die UUID der @link Material_Resource
         * @param path_to_file              Der Pfad der @link Material_Resource
         *
         * @return Material_Resource        Die importierte @link Material_Resource, falls diese erstellt wurde. Andernfalls, std::nullopt.
         */
        static std::optional<Material_Resource> import_Material(const boost::uuids::uuid& uuid,
                                                                const string& path_to_file);

    private:
        /**
         * @brief Methode, um den Inhalt des Vektors indices zu füllen.
         * @return std::vector<int>         Den importierten Inhalt des Vektors indices.
         */
        static std::vector<int> fetch_indices(const boost::uuids::uuid& uuid,
                                              const string& path_to_file);

        /**
         * @brief Methode, um den Inhalt des Vektors materials zu füllen.
         * @return std::vector<Material>    Den importieren Inhalt des Vektors materials.
         */
        static vector<Material> fetch_materials(const string& path_to_file);

        /**
         * @brief Methode, um die Dateiendung zu validieren.
         * @param path_to_file              Der zu überprüfende Dateiname.
         * @param suffix                    Die Dateiendung, auf die geprüft werden soll.
         * @return bool                     True, falls die Datei auf .obj endet. Andernfalls, False.
         */
        static bool validate_extension(const std::string& path_to_file, const std::string& suffix);
};

#endif
