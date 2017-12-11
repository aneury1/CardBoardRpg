#include "pausemenu.h"

PauseMenu::PauseMenu(SDL_Setup* sdlSetup, bool& passedGamePaused, bool& passedRunning) : gamePaused(passedGamePaused), gameLoopRunning(passedRunning)
{
    staticCamera = 0;
    label = new Sprite(sdlSetup->GetRenderer(),"error.png",210,50,4000,200,&staticCamera,&staticCamera);
    label->SetFontSize(70);
    label->SetTextColor(220,220,220,0);
    label->LoadFromText("PAUSED");

    background = new Sprite(sdlSetup->GetRenderer(),"layer.png",0,0,640,400,&staticCamera,&staticCamera);

    resumeButton = new Button(sdlSetup,195,200,"              Resume");
    exitButton = new Button(sdlSetup,195,250,"                 Exit");
}

PauseMenu::~PauseMenu()
{
    delete label;
    delete exitButton;
    delete resumeButton;
    delete background;
}

void PauseMenu::Update()
{
    resumeButton->Update();
    exitButton->Update();

    if(resumeButton->WasClicked())
    {
        gamePaused = false;
    }

    if(exitButton->WasClicked())
    {
        gameLoopRunning = false;
    }
}

void PauseMenu::Draw()
{
    background->Draw();
    label->Draw();
    resumeButton->Draw();
    exitButton->Draw();
}
