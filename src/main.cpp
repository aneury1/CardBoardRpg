#include "game.h"
#include "mainmenu.h"

int main(int argc, char* argv[])
{
    bool wholeProgramRunning = true;
    SDL_Setup sdlSetup;

    MainMenu mainMenu(&sdlSetup);
    Game game(&sdlSetup);

    while(wholeProgramRunning)
    {
        if(mainMenu.Loop(wholeProgramRunning))
        {
            if(game.LoadMap(mainMenu.GetMapName(),mainMenu.GetGameMode()))
            {
                game.GameLoop(wholeProgramRunning);
            }
        }
    }

    //SDL_Delay(1000);

    return 0;
}
