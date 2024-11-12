#ifndef MATERIAL_IMPORTER_H
#define MATERIAL_IMPORTER_H

//TODO Add imports here.
#include "../../Resources/Material/Material_Resource.h"
#include "../../Logger/Logger.h"
#include <string>
#include <vector>

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */
class Material_Importer
{

    public:

        //TODO add public Method definitions.
        Material_Importer();
        ~Material_Importer();

        /**
         * @brief //TODO
         * @param filePath //TODO
         */
        static vector<Material> loadMaterial(const string& filePath);

    private:

        //TODO add private Method definitions.

};

#endif
