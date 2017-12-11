#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "inventory.h"
#include <vector>
#include <fstream>
#include <string>
#include "portal.h"

class Enemy;

class Environment
{
public:
    Environment(SDL_Setup* passedSdlSetup, int* passedCameraX, int* passedCameraY);
    ~Environment();
    void DrawBack(int playerY);
    void DrawFront(int playerY);
    void DrawInvetory();
    void Update();
    void SaveToFile();
    bool LoadFromFile(std::string levelName, GameMode passedGameMode);
    bool IsColliding(Collision* object);

    int GetSpawnX(){return spawnX;}
    int GetSpawnY(){return spawnY;}

    bool PlayerIsInPortal(SDL_Rect playerRect);
    std::string GetPortalsLevelThatThePlayerIsIn(){return (*portalThatThePlayerIsIn)->levelName;}

    bool& PlayerIsAllowedToMove(){return inventory->PlayerIsAllowedToMove();}
private:
    void DeleteEnvironment();
    void SpawnEnemies();

    std::string currentMap;

    int* cameraX;
    int* cameraY;

    SDL_Setup* sdlSetup;
    int spawnX;
    int spawnY;

    std::list<Sprite*> plates;
    std::list<Sprite*> slates;
    std::list<Sprite*> walls;
    std::list<Sprite*> invisibleBorders;
    std::list<Portal*> portals;
    std::list<Portal*>::iterator portalThatThePlayerIsIn;
    std::list<Enemy*> enemies;
    std::list<std::pair<int,int> > enemySpawns;

    std::list<Drawable*> toDraw;
    std::list<Drawable*>::iterator toDrawFrontBegin;//DrawFront starts with this, and DrawBack ends

    GameMode gameMode;

    Inventory* inventory;
};

#endif // ENVIRONMENT_H
