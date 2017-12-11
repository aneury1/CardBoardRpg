#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "sprite.h"
#include "button.h"

class PauseMenu
{
    public:
        PauseMenu(SDL_Setup* sdlSetup, bool& passedGamePaused, bool& passedRunning);
        ~PauseMenu();
        void Update();
        void Draw();
    private:
        Sprite* label;//the game is paused
        Sprite* background;
        Button* resumeButton;
        Button* exitButton;
        bool& gamePaused;
        bool& gameLoopRunning;

        int staticCamera;
};

#endif // PAUSEMENU_H
