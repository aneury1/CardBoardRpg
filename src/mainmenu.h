#ifndef MAINMENU_H
#define MAINMENU_H
#include "button.h"
#include "textfield.h"


class MainMenu
{
    public:
        MainMenu(SDL_Setup* passedSdlSetup);
        ~MainMenu();
        bool Loop(bool& wholeProgramRunning);//true if button pressed
        std::string GetMapName(){return mapName->GetText();}
        GameMode GetGameMode(){return gameMode;}
    private:
        void ResetTheLoop(){running = true;}

        Sprite* background;
        Sprite* title;

        TextField* mapName;
        Button* playButton;
        Button* editButton;
        Button* newButton;

        SDL_Setup* sdlSetup;//new SDL_Setup
        bool running;
        int staticCamera;

        GameMode gameMode;
};

#endif // MAINMENU_H
