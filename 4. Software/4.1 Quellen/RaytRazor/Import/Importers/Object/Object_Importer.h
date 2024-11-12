#ifndef OBJECT_IMPORTER_H
#define OBJECT_IMPORTER_H

//TODO Add imports here.
#include "../../Resources/Object/Object_Resource.h"
#include "../../Logger/Logger.h"
#include <string>
#include <vector>

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */
class Object_Importer
{

    public:

        //TODO add public Method definitions.
        Object_Importer();
        ~Object_Importer();

        /**
         * @brief //TODO
         * @param filePath //TODO
         */
        static vector<Vertex> loadObject(const string& filePath);

    private:

        //TODO add private Method definitions.

};

#endif
