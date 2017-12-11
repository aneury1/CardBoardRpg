#ifndef SDL_SETUP_H
#define SDL_SETUP_H

#include "rpg.h"
#include "messagebox.h"

class SDL_Setup
{
public:
    SDL_Setup();
    ~SDL_Setup();
    SDL_Renderer* GetRenderer();
    SDL_Event* GetMainEvent();
    SDL_Rect GetWindowRect();

    bool KeyWasPressed(int key);
    bool MouseButtonLeftWasPressed();

    void Begin();
    void End();

    void ShowMessage(std::string message){messageBox->PopUp(message);}
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event* mainEvent;

    bool newEventHappened;

    MessageBox* messageBox;
};

#endif // SDL_SETUP_H
