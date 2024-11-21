#ifndef JSON_PARSER_H
#define JSON_PARSER_H

//TODO Add imports here.
#include <../../../Utility/External_Libraries/nlohmann/json.hpp>
#include "../Import/Resources/Material/Material_Resource.h"
#include "../Import/Resources/Object/Object_Resource.h"
#include "../Components/Camera/Camera_Component.h"
#include "../Components/Entities/Render_Component.h"
#include "../Components/Lighting/Light_Component.h"
#include <string>
#include <map>

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */
class Json_Parser
{

    public:

        Json_Parser() = delete;
        ~Json_Parser();

        /**
         * @brief //TODO
         * @param pathToJson //TODO
         */
        static void parseJSON(const string& pathToJson);

        /**
         * @brief //TODO
         * @param exportPath //TODO
         */
        static void exportToJSON(const string& exportPath);

        //TODO Implements Getter-Methods for map contents.

    private:

        map<int /*TODO MAKE THIS FROM INT TO UUID*/, Camera_Component> cameras;
        map<int /*TODO MAKE THIS FROM INT TO UUID*/, Light_Component> lights;
        map<int /*TODO MAKE THIS FROM INT TO UUID*/, Render_Component> renders;
        map<int /*TODO MAKE THIS FROM INT TO UUID*/, Object_Resource> objects;
        map<int /*TODO MAKE THIS FROM INT TO UUID*/, Material_Resource> materials;

};

#endif
