#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <../../../Utility/External_Libraries/nlohmann/json.hpp>
#include "../Import/Resources/Material/Material_Resource.h"
#include "../Import/Resources/Object/Object_Resource.h"
#include "../Components/Base_Component.h"
#include <boost/uuid/uuid.hpp>
#include <string>
#include <map>

using namespace std;

/**
 * @brief Diese Klasse bietet Funktionalität, um 3D-Szenenbeschreibungen im JSON Format einzulesen.
 * @author Christian Kasper, Lukas Jeckle
 */
class Json_Parser
{

    public:

        Json_Parser() = delete;
        ~Json_Parser();

        /**
         * @brief     Nimmt die JSON-Datei einer 3D-Szenenbeschreibung entgegen und liest die darin gespeicherten Werte als
         *            Key-Value Paare, bestehend aus UUID's und Components, ein.
         *
         * @param pathToJson          Der Pfad, an dem die zu parsende JSON-Datei zu finden ist. (e.g.: "./JSON/3D_Szene.json"
         * @return                    Eine Map, die den Inhalt der JSON-Datei als Key-Value Paare aus UUID's und Components enthält.
         */
        static map<boost::uuids::uuid, Base_Component> parseJSON(const string& pathToJson) const;

        /** //TODO Evtl. Make this part of SceneManager?
         * @brief Exportiert den aktuellen Inhalt der 3D-Szene und speichert diesen an dem übergebenen Export-Pfad.
         * @param exportPath           Der Pfad, an dem die zu exportierende JSON-Datei gespeicher werden soll.
         * @return                     True, wenn Export erfolgreich war. Andernfalls, false.
         */
        static boolean exportToJSON(const string& exportPath) const;

    /*private: //TODO Make this part of the SceneManager.

        map<boost::uuids::uuid, Camera_Component> cameras;
        map<boost::uuids::uuid, Light_Component> lights;
        map<boost::uuids::uuid, Render_Component> renders;
        map<boost::uuids::uuid, Object_Resource> objects;
        map<boost::uuids::uuid, Material_Resource> materials;*/

};

#endif
