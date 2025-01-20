//
// Created by leonm on 12/2/2024.
//

#ifndef RT_APP_H
#define RT_APP_H

#include <SDL.h>
#include "RT_Image.h"
#include "RT_Scene.h"

/**
 * @brief Klasse RayTrace_App ist zuständig für die Darstellung des SDL2 Fensters.
 * @author Leon Musliu
 */
class CApp
{
public:
    /** @brief Default Konstruktor der App, setzt isRunning auf true und initialisiert Fenster & Renderer*/
    CApp();
    /**
     * @brief Main funktion der Applikation, beinhaltet die Logik während die App läuft.
     * @returns Integer. 0 bei erfolgreicher Ausführung, -1 bei Fehler. Kann erweitert werden.
     */
    int OnExecute();
    /**
     * @brief Initialisiert die Applikation. Wird von OnExecute() genutzt.
     * @returns Boolean. true bei erfolgreicher Ausführung, false bei Fehler.
     */
    bool OnInit();
    /**
     * @brief Handled die Events die während der Ausführung passieren können. Wird in der Mainloop aufgerufen.
     * @param event Der momentane Event welcher gefangen wurde.
     */
    void OnEvent(const SDL_Event *event);
    /** @brief Beinhaltet Logik, was während der Ausführung passiert. Ist hier jedoch leer, da nur das Bild angezeigt wird.*/
    static void OnLoop();
    /** @brief Beinhaltet Logik, was beim Rendern passiert.*/
    static void OnRender();
    /** @brief Schließt das SDL2 Fenster.*/
    void OnExit();

private:
    Image m_image;
    RT_Scene m_scene;
    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;

};

#endif //RT_APP_H
