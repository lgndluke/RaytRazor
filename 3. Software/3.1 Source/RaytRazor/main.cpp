#include "UI/Scenes/Main/Main_Scene.h"
#include "Utility/Logger/Logger.h"
#include <nanogui/nanogui.h>
#include <iostream>
#include <map>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Parsing/Json_Parser.h"

using namespace nanogui;

int main(int argc, char* argv[]) {

    try
    {
        init();
        nanogui::ref app = new Main_Scene(1400, 700, "RaytRazor", true);
        app->drawAll();
        app->setVisible(true);
        mainloop();
        shutdown();
    }
    catch (const std::runtime_error &e)
    {
        Logger::log(MessageType::SEVERE, "main(): Caught fatal error: " + std::string(e.what()));
        return -1;
    }

    return 0;
}
