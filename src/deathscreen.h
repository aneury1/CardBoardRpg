#ifndef DEATHSCREEN_H
#define DEATHSCREEN_H
#include "sprite.h"
#include "button.h"
class DeathScreen
{
    public:
        DeathScreen(SDL_Setup* sdlSetup, bool& passedRunning);
        ~DeathScreen();
        void Update();
        void Draw();
    private:
        Sprite* label;
        Sprite* background;
        Button* exitButton;

        bool& gameLoopRunning;

        int staticCamera;
};

#endif // DEATHSCREEN_H
