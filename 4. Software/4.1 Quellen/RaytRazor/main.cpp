#include "UI/Scenes/Main/Main_Scene.h"
#include "Utility/Logger/Logger.h"
#include <nanogui/nanogui.h>

using namespace nanogui;

int main(int argc, char* argv[]) {

    try
    {
        init();
        nanogui::ref app = new Main_Scene(1920, 1010, "RaytRazor", true);
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