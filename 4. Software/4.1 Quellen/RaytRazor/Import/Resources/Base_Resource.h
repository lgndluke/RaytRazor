#ifndef BASE_RESOURCE_H
#define BASE_RESOURCE_H

#include <string>
#include <boost/uuid/uuid.hpp>

using namespace std;

/**
 * @brief Enum das die unterschiedlichen Ressourcen Typen definiert.
 *
 * @code MATERIAL@endcode - um 'MATERIAL' Objekte zu beschreiben.
 * @code OBJECT  @endcode - um 'OBJECT'   Objekte zu beschreiben.
 *
 * @author Lukas Jeckle
 */
enum ResourceType
{
    MATERIAL,
    OBJECT
};

/**
 * @brief Abstrakte Basis Klasse für Resource Objekte.
 * @author Lukas Jeckle
 */
class Base_Resource
{

    public:

        /**
         * @brief Konstruktor zum Erstellen eines Base_Resource Objekts.
         *
         * @param uuid                   UUID der Ressource.
         * @param type                   Typ der Ressource.
         * @param path                   Pfad der Ressource.
         */
        Base_Resource(const boost::uuids::uuid uuid, const ResourceType type, const string& path);

        Base_Resource() = default;
        /**
         * @brief Methode, um die UUID der Ressource zu erhalten.
         * @return boost::uuid::uuid     UUID der Ressource.
         */
        [[nodiscard]] boost::uuids::uuid get_uuid() const;

        /**
         * @brief Methode, um den Typ der Ressource zu erhalten.
         * @return ResourceType         Typ der Ressource.
         */
        [[nodiscard]] ResourceType get_type() const;

        /**
         * @brief Methode, um den Pfad der Ressource zu erhalten.
         * @return string               Pfad der Ressource.
         */
        [[nodiscard]] string get_path() const;

        /**
         * @brief Methode, um den Pfad der Ressource zu ändern.
         * @param new_path              Neuer Pfad zur Ressource.
         */
        void set_path(const string& new_path);

    private:

        boost::uuids::uuid uuid;
        ResourceType type;
        string path;

};

#endif
