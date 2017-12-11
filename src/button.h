#ifndef BUTTON_H
#define BUTTON_H

#include "sprite.h"
#include "sdl_setup.h"
#include "soundeffect.h"

enum ButtonState
{
    BS_UNCLICKED,
    BS_MOUSE_OVER,
    BS_CLICKED
};

class Button
{
    public:
        Button(SDL_Setup* passedSdlSetup, int x, int y, std::string buttonText);
        ~Button();
        void Update();
        void Draw();
        bool WasClicked(){return wasClicked;}
    private:
        ButtonState buttonState;
        Sprite* buttonTextureClicked;
        Sprite* buttonTextureUnclicked;
        Sprite* buttonTextureMouseOver;

        bool wasClicked;

        std::string text;
        Sprite* textSprite;

        SoundEffect* clickSound;

        SDL_Setup* sdlSetup;
        int staticCamera;
};

#endif // BUTTON_H
