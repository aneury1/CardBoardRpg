#include "mainmenu.h"
#include <fstream>

MainMenu::MainMenu(SDL_Setup* passedSdlSetup)
{
    running = true;
    sdlSetup = passedSdlSetup;
    staticCamera = 0;

    mapName = new TextField(195,130,250,60,sdlSetup,"Map Name: ");
    playButton = new Button(sdlSetup,195,200,"               Play");
    editButton = new Button(sdlSetup,195,250,"               Edit");
    newButton = new Button(sdlSetup,195,300,"               New");

    background = new Sprite(sdlSetup->GetRenderer(),"background.png",0,0,640,400,&staticCamera,&staticCamera);
    title = new Sprite(sdlSetup->GetRenderer(),"title.png",40,0,560,113,&staticCamera,&staticCamera);
}

MainMenu::~MainMenu()
{
    delete mapName;
    delete background;
    delete title;
    delete playButton;
    delete editButton;
    delete newButton;
}

bool MainMenu::Loop(bool& wholeProgramRunning)
{
    ResetTheLoop();
    while(running)
    {
        if(sdlSetup->GetMainEvent()->type == SDL_QUIT)
        {
            wholeProgramRunning = false;
            running = false;
        }

        sdlSetup->Begin();

        background->Draw();
        title->Draw();

        mapName->Update();
        mapName->Draw();
        playButton->Update();
        editButton->Update();
        newButton->Update();
        playButton->Draw();
        editButton->Draw();
        newButton->Draw();

        if((playButton->WasClicked() || sdlSetup->KeyWasPressed(SDL_SCANCODE_RETURN)) && !mapName->GetText().empty())
        {
            gameMode = GM_GAMEPLAY;
            running = false;//convenience
            return true;
        }
        if(editButton->WasClicked() && !mapName->GetText().empty())
        {
            gameMode = GM_LEVELEDIT;
            running = false;//convenience
            return true;
        }
        if(newButton->WasClicked() && !mapName->GetText().empty())
        {
            if(CheckLevelForExistance(mapName->GetText()))
            {
                sdlSetup->ShowMessage("Level '" + mapName->GetText() + "' already exists!");
            }
            else
            {
                std::string newLevelPath = "data/maps/" + mapName->GetText() + ".txt";
                std::ofstream newLevel(newLevelPath.c_str());
                newLevel << "SPAWN: 0 0 ;" << std::endl << "END";
                sdlSetup->ShowMessage("Level '" + mapName->GetText() + "' created!");

                gameMode = GM_LEVELEDIT;
                running = false;//convenience
                return true;
            }
        }

        sdlSetup->End();
    }
    return false;//compiler was complaining
}
