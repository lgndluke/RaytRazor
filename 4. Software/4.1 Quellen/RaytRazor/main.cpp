#include "Parsing/Json_Parser.h"
#include "Components/Base_Component.h"
#include "Import/Resources/Base_Resource.h"
#include <boost/uuid.hpp>
#include <map>
#include <vector>

int main()
{
    map<boost::uuids::uuid, Base_Component> components;
    map<boost::uuids::uuid, Base_Resource> resources;

    const string path = ""; //"Parsing/JsonParser_DummyFile.json"?

    Json_Parser::parseJSON(path, components, resources);

    printf("Done.");
}