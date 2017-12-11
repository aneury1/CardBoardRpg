#ifndef GAME_H
#define GAME_H

#include "maincharacter.h"
#include "pausemenu.h"
#include "deathscreen.h"
#include "enemy.h"
#include "music.h"

class Game
{
public:
    Game(SDL_Setup* passedSdlSetup);
    ~Game();
    void GameLoop(bool& wholeProgramRunning);
    bool LoadMap(std::string mapName, GameMode passedGameMode);
private:
    void ResetTheGameLoop();

    bool running;
    bool paused;

    SDL_Setup* sdlSetup;

    Music* music;

    GameMode gameMode;

    PauseMenu* pauseMenu;
    DeathScreen* deathScreen;

    int cameraX;
    int cameraY;

    //pass this to Sprite's constructor if do not want it to offset from the screen like tree or something, e.g toolbar
    int staticCamera;

    Environment* environment;

    //an image imitating lightning
    Sprite* shade;

    MainCharacter* player;
};

#endif // GAME_H
