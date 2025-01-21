#include "Fragment_Shader.h"

string Fragment_Shader::get_fragment_shader()
{

    return R"(

        #version 330 core
        in vec4 frag_color;
        out vec4 color;
        void main()
        {
            color = frag_color;
        }

    )";

}