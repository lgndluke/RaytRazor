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
    * @brief Überschreibt die KeyboardEvents an der Preview
    * @param key gedrückte Taste
    * @param scancode
    * @param action
    * @param modifiers sowas wie Control und STRG
    * @author Christian Kasper
    */
    bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
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
    static glm::mat4 calculateViewDir(glm::vec3 rotation);

    /**
    * @brief zum berechnung des punktes auf die die kamera zeigt und den UP vector
    * @param rotation                Rotations vektor des objektes.
    * @param rotation                Position des objektes.
    */
    static std::pair<glm::vec3,glm::vec3> calculateCameraVectors(glm::vec3 position, glm::vec3 rotation);

    /**
    * @brief
    * @param input                Render_Component dessen Model matrix man braucht.
    */
    static glm::mat4 extract_Model_Matrix(const shared_ptr<Base_Component>& input);

    static Eigen::MatrixXf make_sphere_vertices();

    static nanogui::MatrixXu make_sphere_indices();

private:
    GLShader mShader;
    Eigen::Vector3f mRotation{0.25f, 0.5f, 0.33f};
    shared_ptr<Base_Component> camera_glob;
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

        /**
        * @brief Überschreibt die KeyboardEvents an der Main_Scene
        * @param key gedrückte Taste
        * @param scancode
        * @param action
        * @param modifiers sowas wie Control und STRG
        * @author Christian Kasper
        */
        bool keyboardEvent(int key, int scancode, int action, int modifiers) override;


        static map<boost::uuids::uuid, shared_ptr<Base_Component>> getComponents()
        {
            return components;
        }
        static map<boost::uuids::uuid, shared_ptr<Base_Resource>> getResources()
        {
            return resources;
        }

        static float getScalingFactor();

        static void setChangesOnComponent(const std::shared_ptr<Base_Component>& component);

        /**
        * @brief Öffnet eine ausgewählte Szene im JsonParser und generiert dadurch ressourcen und components
        * @author Christian Kasper
        */
        static void openScene();

        /**
        * @brief Fügt neue Componenenten zu Szene hinzu
        * @param uuid, Identifier
        * @param component, das zu hinzufügende Componente
        * @author Christian Kasper
        */
        static void addComponent(const boost::uuids::uuid& uuid, const std::shared_ptr<Base_Component>& component);

        static void removeComponent(const boost::uuids::uuid& uuid);

         /**
        * @brief Fügt neue Componenenten zu Szene hinzu
        * @param uuid, Identifier
        * @param object_resource, die zu hinzufügende Resource
        * @author Christian Kasper
        */
        static void addResource(boost::uuids::uuid uuid, const std::shared_ptr<Base_Resource>& object_resource);

    static void forceUpdate();

    private:

        int window_width;
        int window_height;
        string window_title;
        bool is_resizeable;
        string scene_path;
        static float scaling;
        //Widgets
        TreeView_Widget* tree_view;
        ComponentAttributes_Widget* attributesWidget;

        /**
        * @brief Singleton von der Szene MainSzene
        * @author Christian Kasper
        */
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

        /**
        * @brief Öffnet eine FileDialog von Windows
        * @author Christian Kasper
        */
        static std::string openFileDialog();

        /**
        * @brief Überprüft ob die Endung .json ist und vertauscht '\\' mit '/'
        * @author Christian Kasper
        */
        static bool isJsonFileAndFixPath(std::string& path);

        /**
        * @brief Ruft eine Update Methode im TreeView auf um neue Nodes anzuzeigen
        * @author Christian Kasper
        */
        void updateTreeView() const;
};

#endif
