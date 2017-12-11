#include "deathscreen.h"

DeathScreen::DeathScreen(SDL_Setup* sdlSetup, bool& passedRunning):gameLoopRunning(passedRunning)
{
    staticCamera = 0;
    label = new Sprite(sdlSetup->GetRenderer(),"error.png",130,50,4000,200,&staticCamera,&staticCamera);
    label->SetFontSize(70);
    label->SetTextColor(220,220,220,0);
    label->LoadFromText("YOU ARE DEAD");

    background = new Sprite(sdlSetup->GetRenderer(),"layer.png",0,0,640,400,&staticCamera,&staticCamera);

    exitButton = new Button(sdlSetup,195,300,"                 Exit");
}

DeathScreen::~DeathScreen()
{
    delete label;
    delete exitButton;
    delete background;
}

void DeathScreen::Update()
{
    exitButton->Update();

    if(exitButton->WasClicked())
    {
        gameLoopRunning = false;
    }
}

void DeathScreen::Draw()
{
    background->Draw();
    label->Draw();
    exitButton->Draw();
}
