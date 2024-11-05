//
// Created by blau08 on 04.11.2024.
//

// SceneParser.h
#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <string>
#include <optional>
#include <map>
#include <iostream>

#include "Components/Entity.h"
#include "Components/Resources/MaterialResource.h"
#include "Components/Resources/ObjectResource.h"


// Resources-Klasse
struct Resources {
    std::map<int, ObjectResource> objects;
    std::map<int, MaterialResource> materials;
};

// Scene-Klasse
struct Scene {
    std::map<std::string, Entity> entities;
    Resources resources;
};

/**
 * @brief Funktion zum Parsen einer Szene aus JSON-Daten.
 *
 * Diese Funktion nimmt einen JSON-String entgegen, der die Struktur einer
 * Szene beschreibt, und konvertiert diesen in ein `Scene`-Objekt.
 * Sie extrahiert die Entitäten und deren Eigenschaften sowie die
 * Ressourcen, die für die Entitäten benötigt werden.
 *
 * @param jsonData Ein String, der die JSON-Daten für die Szene enthält.
 * @return Scene Ein Objekt, das die geparste Szene darstellt,
 * einschließlich ihrer Entitäten und Ressourcen.
 */
Scene parseScene(const std::string& jsonData);

/**
 * @brief Exportiert eine Szene in das JSON-Format.
 *
 * Diese Funktion konvertiert ein gegebenes `Scene`-Objekt in einen
 * JSON-String, der die Struktur und Eigenschaften der Szene darstellt.
 * Sie eignet sich zum Speichern von Szenendaten in einer Datei für
 * die spätere Verwendung oder zur Übertragung zwischen Anwendungen.
 *
 * @param scene Das `Scene`-Objekt, das exportiert werden soll.
 * @param filePath Der Pfad zur Datei, in die die JSON-Daten geschrieben werden.
 * @return bool Gibt true zurück, wenn der Export erfolgreich war,
 *              andernfalls false, wenn ein Fehler aufgetreten ist.
 */
std::string exportSceneToJson(const Scene& scene);
#endif // SCENEPARSER_H
