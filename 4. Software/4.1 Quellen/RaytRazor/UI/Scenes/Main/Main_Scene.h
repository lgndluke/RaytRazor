#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H
// muss ganz oben sein son "Object ambigious"
#include "../../../Raytracer/RT_App.h"

#include "../../../Components/Base_Component.h"
#include "../../../Import/Resources/Base_Resource.h"
#include "../../../Parsing/Json_Parser.h"
#include "../../../Utility/Logger/Logger.h"
#include "../../Widget/MenuBar_Widget.h"
#include "../../Widget/ComponentAttributes_Widget.h"
#include "../../Widget/TreeView_Widget.h"
#include "../../../Shaders/Fragment/Fragment_Shader.h"
#include "../../../Shaders/Vertex/Vertex_Shader.h"
#include "../../utility/Custom_Label.h"
#include <commdlg.h>
#include <thread>
#include "../../../Converter/Converter.h"
#include "../../../Import/Importers/Object/Object_Importer.h"
#include "../../../Import/Importers/Material/Material_Importer.h"
#include <boost/uuid.hpp>
#include <nanogui/nanogui.h>
#include <vector>
#include <map>

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

        /**
        * @brief zum berechnen der richtung in die das objekt (normalerweise kamera) zeigt
        * @param rotation                Rotations vektor des objektes.
        */
        glm::vec3 calculateViewDir(glm::vec3 rotation);

        /**
        * @brief zum berechnung des punktes auf die die kamera zeigt und den UP vector
        * @param rotation                Rotations vektor des objektes.
        * @param rotation                Position des objektes.
        */
        std::pair<glm::vec3,glm::vec3> calculateCameraVectors(glm::vec3 position, glm::vec3 rotation);

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

        static map<boost::uuids::uuid, shared_ptr<Base_Component>> getComponents()
        {
            return components;
        }
        static map<boost::uuids::uuid, shared_ptr<Base_Resource>> getResources()
        {
            return resources;
        }

        static void addComponent(const boost::uuids::uuid& uuid, const std::shared_ptr<Base_Component>& component);

    static void forceUpdate();

    private:

        int window_width;
        int window_height;
        string window_title;
        bool is_resizeable;

        //Widgets
        TreeView_Widget* tree_view;
        ComponentAttributes_Widget* attributesWidget;

        static Main_Scene* instance;

        vector<int> ids;
        static map<boost::uuids::uuid, shared_ptr<Base_Component>> components;
        static map<boost::uuids::uuid, shared_ptr<Base_Resource>> resources;

        /**
         * @brief Methode, um die Main Szene zu initialisieren.
         */
        void initialize();

        /**
         * @brief Methode, um die 3D-Preview der Main Szene mittels raytracing zu rendern.
         * @return nullptr
         */
        void* (*raytrace_preview())(void*);

        static std::string openFileDialog();

        static bool isJsonFileAndFixPath(std::string& path);

        void updateTreeView() const;
};

#endif
