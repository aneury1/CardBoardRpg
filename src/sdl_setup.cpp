#include "sdl_setup.h"

SDL_Setup::SDL_Setup()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);

    SDL_StartTextInput();
    window = NULL;
    window = SDL_CreateWindow("Cardboard rpg",100,100,640,400,SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        DebugCOut("Can't create the Window");
    }

    renderer = NULL;
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        DebugCOut("Can't create the Renderer");
    }

    mainEvent = new SDL_Event();

    newEventHappened = false;

    messageBox = new MessageBox(renderer);
}

SDL_Setup::~SDL_Setup()
{
    delete messageBox;

    SDL_StopTextInput();

    delete mainEvent;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}

SDL_Renderer* SDL_Setup::GetRenderer()
{
    return renderer;
}

SDL_Event* SDL_Setup::GetMainEvent()
{
    return mainEvent;
}

void SDL_Setup::Begin()
{
    if(SDL_PollEvent(GetMainEvent()) == 1)
    {
        newEventHappened = true;
    }
    else
    {
        newEventHappened = false;
    }

    SDL_RenderClear(GetRenderer());

    messageBox->Update();
}

void SDL_Setup::End()
{
    messageBox->Draw();

    SDL_RenderPresent(GetRenderer());
    SDL_Delay(10);
}

SDL_Rect SDL_Setup::GetWindowRect()
{
    SDL_Rect windowRect;
    windowRect.x=0;
    windowRect.y=0;
    windowRect.w=0;
    windowRect.h=0;
    SDL_GetWindowSize(window,&windowRect.w,&windowRect.h);
    SDL_GetWindowPosition(window,&windowRect.x,&windowRect.y);

    return windowRect;
}

bool SDL_Setup::KeyWasPressed(int key)
{
    if(newEventHappened)
    {
        if(GetMainEvent()->type == SDL_KEYDOWN)
        {
            if(GetMainEvent()->key.keysym.scancode == key)
            {
                return true;
            }
        }
    }

    return false;
}

bool SDL_Setup::MouseButtonLeftWasPressed()
{
    if(newEventHappened)
    {
        if(GetMainEvent()->type == SDL_MOUSEBUTTONDOWN)
        {
            if(GetMainEvent()->button.button == SDL_BUTTON_LEFT)
            {
                return true;
            }
        }
    }

    return false;
}
