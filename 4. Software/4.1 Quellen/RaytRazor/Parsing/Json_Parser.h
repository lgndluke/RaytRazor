#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "../Components/Base_Component.h"
#include "../Components/Camera/Camera_Component.h"
#include "../Components/Entities/Render_Component.h"
#include "../Components/Lighting/Light_Component.h"
#include "../Import/Resources/Material/Material_Resource.h"
#include "../Import/Resources/Object/Object_Resource.h"
#include "../Utility/Logger/Logger.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

/**
 * @brief Klasse mit Funktionalitäten bezüglich Json 3D-Szenenbeschreibungen.
 * @author Christian Kasper, Lukas Jeckle
 */
class Json_Parser
{

    public:
        /**
         * @brief Statische Klasse ohne Konstruktor.
         */
        Json_Parser() = delete;

        /**
         * @brief Methode, um Json 3D-Szenenbeschreibungen einzulesen.
         * @param path_To_Json              Pfad zur entsprechenden 3D-Szenenbeschreibung.
         * @param components                Referenz auf die zu verwendende Base_Components Map.
         * @param resources                 Referenz auf die zu verwendende Base_Resource Map.
         */
        static void parseJSON(const string& path_To_Json,
                              map<boost::uuids::uuid, shared_ptr<Base_Component>>& components,
                              map<boost::uuids::uuid, shared_ptr<Base_Resource>>& resources);

        /**
        * @brief Exportiert die übergebenen Komponenten und Ressourcen in eine JSON-Datei.
        *
        * Diese Methode nimmt eine Sammlung von Komponenten und Ressourcen und generiert
        * eine strukturierte JSON-Datei, die die Details dieser Objekte enthält. Die
        * Komponenten können von verschiedenen Typen sein (z. B. Render_Component, Light_Component)
        * und werden basierend auf ihrem Typ in entsprechende JSON-Abschnitte aufgeteilt.
        *
        * @param exportPath Der Dateipfad, unter dem die JSON-Datei gespeichert werden soll.
        * @param components Eine Map mit UUIDs als Schlüsseln und verschiedenen Komponenten
        *                   (Base_Component, Render_Component, Light_Component, Camera_Light) als Werten.
        * @param resources  Eine Map mit UUIDs als Schlüsseln und Ressourcen (Base_Resource) als Werten.
        *
        * @return true, wenn die JSON-Datei erfolgreich exportiert wurde, andernfalls false.
        *
        * @details
        * - Komponenten werden in spezifische JSON-Arrays wie "renderentity", "lightentity"
        *   und "cameraentity" unterteilt.
        * - Ressourcen werden in einem separaten "resources"-Array abgelegt.
        * - Jede Komponente und Ressource wird anhand ihrer UUID und spezifischen Eigenschaften
        *   in der JSON-Struktur repräsentiert.
        * - Falls ein Fehler beim Schreiben der Datei auftritt, wird dieser protokolliert.
        *
        * @note Diese Funktion verwendet externe Bibliotheken wie nlohmann::json für die JSON-Verarbeitung
        *       und boost::uuids für die Verwaltung von UUIDs.
        * @author Christan Kasper, Dennis Welsch
        */
        static bool exportToJSON(const std::string& exportPath,
                             const std::map<boost::uuids::uuid, variant<Base_Component, Render_Component, Light_Component, Camera_Component>> components,
                             const std::map<boost::uuids::uuid, Base_Resource>& resources);

};

#endif
