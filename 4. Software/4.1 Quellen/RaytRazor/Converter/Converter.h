#ifndef CONVERTER_H
#define CONVERTER_H


#include "../Import/Resources/Object/Object_Resource.h"
#include <nanogui/nanogui.h>
#include <vector>

/**
 * @brief //TODO
 * @author
 */
class Converter {

    public:


    Converter() = delete;

    nanogui::MatrixXu Convert_to_MatIndices(const Object_Resource& source){};

    nanogui::MatrixXf Convert_to_MatVertices(const Object_Resource& source){};

    nanogui::MatrixXf Convert_to_MatColor(const Object_Resource& source){};

    private:

};

#endif
