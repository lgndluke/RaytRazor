#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "../../../Components/Base_Component.h"
#include "../../../Import/Resources/Base_Resource.h"
#include "../../../Utility/Logger/Logger.h"
#include "../../../Raytracer/RT_App.h"
#include <nanogui/nanogui.h>
#include <vector>
#include <map>
#include "../../../Shaders/Fragment/FragmentShader.h"
#include "../../../Shaders/Vertex/VertexShader.h"

using namespace std;
using namespace nanogui;

/**
 * @brief Abgeleitete Klasse FixedWindow repräsentiert ein nicht bewegbares Fenster in der Main_Scene.
 * @author Lukas Jeckle
 */
class Fixed_Window final : public Window
{
    public:

        /**
         * @brief Konstruktor zum Erstellen eines Fixed_Window Objekts.
         */
        Fixed_Window(Widget *parent, const std::string &title);

        /**
         * @brief Überschriebene Methode der Klasse Window um Mouse-Drag Events zu deaktivieren.
         */
        bool mouseDragEvent(const Vector2i& p, const Vector2i &rel,
                            int button, int /* modifiers */) override;

};

/**
 * @brief Abgeleitete Klasse Preview_Canvas repräsentiert die OpenGL Preview in der Main_Scene.
 * @author Lukas Jeckle, Dennis Welsch
 */
class Preview_Canvas final : public GLCanvas
{

    public:

        /**
         * @brief Konstruktor zum Erstellen eines Preview_Canvas Objekts.
         * @param parent                Der Eltern-Widget des Canvas.
         */
        explicit Preview_Canvas(Widget* parent);

        /**
         * @brief Methode, um den GLCanvas der Main Szene zu zeichnen.
         */
        void drawGL() override;

    private:
        GLShader mShader;
        Eigen::Vector3f mRotation{0.25f, 0.5f, 0.33f};
};

/**
 * @brief Die Klasse Main_Scene repräsentiert das Haupt-Fenster der Anwendung.
 * @author Lukas Jeckle
 */
class Main_Scene final : public Screen
{

    public:

        /**
         * @brief Konstruktor zum Erstellen eines Main_Scene Objektes.
         *
         * @param window_width         Die Breite des zu erstellenden Fensters.
         * @param window_height        Die Höhe des zu erstellenden Fensters.
         * @param window_title         Der Titel des zu erstellenden Fensters.
         * @param is_resizeable        Statische Fenstergröße (false), Dynamische Fenstergröße (true)
         */
        Main_Scene(const int window_width,
                   const int window_height,
                   const string& window_title,
                   const bool is_resizeable);

        /**
         * @brief Methode, um die Main_Scene zu aktualisieren.
         */
        void update();

    private:

        int window_width;
        int window_height;
        string window_title;
        bool is_resizeable;

        vector<int> ids;
        map<int, Base_Component> components;
        map<int, Base_Resource> resources;

        /**
         * @brief Methode, um die Main Szene zu initialisieren.
         */
        void initialize();

        /**
         * @brief Methode, um die 3D-Preview der Main Szene mittels raytracing zu rendern.
         * @return nullptr
         */
        void* (*raytrace_preview())(void*);

};

#endif
