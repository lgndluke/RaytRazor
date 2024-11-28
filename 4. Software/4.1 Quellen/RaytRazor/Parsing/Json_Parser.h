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
                              map<boost::uuids::uuid, Base_Component>& components,
                              map<boost::uuids::uuid, Base_Resource>& resources);

        /**
         *
         * @param exportPath
         * @return bool
         */
        static bool exportToJSON(const string& exportPath);

};

#endif
