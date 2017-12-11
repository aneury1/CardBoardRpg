#include "game.h"

Game::Game(SDL_Setup* passedSdlSetup)
{
    running = true;
    paused = false;

    cameraX = 0;
    cameraY = 0;

    staticCamera = 0;

    sdlSetup = passedSdlSetup;

    pauseMenu = new PauseMenu(sdlSetup,paused,running);
    deathScreen = new DeathScreen(sdlSetup,running);

    environment = new Environment(sdlSetup, &cameraX, &cameraY);

    shade = new Sprite(sdlSetup->GetRenderer(),std::string("layer.png"),0,0,640,400, &staticCamera, &staticCamera);

    player = NULL;

    music = new Music("JewelBeat_Final_Quest.wav");
}

Game::~Game()
{
    delete music;
    delete pauseMenu;
    delete deathScreen;

    if(player != NULL)
    {
        delete player;
    }
    delete environment;
    delete shade;
}

void Game::GameLoop(bool& wholeProgramRunning)
{
    ResetTheGameLoop();
    if(gameMode == GM_GAMEPLAY)
        music->Play(100);

    while(running)
    {
        sdlSetup->Begin();

        if(sdlSetup->GetMainEvent()->type == SDL_QUIT)
        {
            wholeProgramRunning = false;
            running = false;
        }
        if(!player->Dead())
        {
            if(sdlSetup->KeyWasPressed(SDL_SCANCODE_ESCAPE))
            {
                paused = !paused;
            }
            if(paused)
            {
                pauseMenu->Update();
            }
            else
            {
                environment->Update();

                player->Update();
                player->UpdateCamera();

                if(environment->PlayerIsInPortal(player->GetRect()))
                {
                    delete player;
                    player = NULL;
                    LoadMap(environment->GetPortalsLevelThatThePlayerIsIn(), gameMode);
                }
            }
        }

        environment->DrawBack(player->GetDownY());

        if(!player->Dead())
        {
            player->Draw();
        }

        environment->DrawFront(player->GetDownY());

        shade->Draw();

        if(!player->Dead())
        {
            if(paused)
            {
                pauseMenu->Draw();
            }
            else if(gameMode == GM_LEVELEDIT)
            {
                environment->DrawInvetory();
            }
            else if(gameMode == GM_GAMEPLAY)
            {
                player->DrawHealthBar();
            }
        }
        else
        {
            deathScreen->Update();
            deathScreen->Draw();
        }
        sdlSetup->End();
    }
    if(gameMode == GM_GAMEPLAY)
        music->Stop();

    delete player;
    player = NULL;
}

bool Game::LoadMap(std::string mapName,GameMode passedGameMode)
{
    gameMode = passedGameMode;
    if(environment->LoadFromFile(mapName, gameMode))
    {
        if(player == NULL)
        {
            player = new MainCharacter(sdlSetup, &cameraX, &cameraY, environment, gameMode);
            Enemy::SetPlayerPointer(player);
        }
        player->TeleportTo(environment->GetSpawnX(),environment->GetSpawnY());//spawn probably changed
        return true;
    }
    return false;
}

void Game::ResetTheGameLoop()
{
    running = true;
    paused = false;
}
