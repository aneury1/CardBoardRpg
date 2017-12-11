#ifndef INVENTORY_H
#define INVENTORY_H

#include "sdl_setup.h"
#include "item.h"
#include "textfield.h"
#include <list>
#include "portal.h"

class Enemy;

enum ITEMS
{
    ITEM_PLATE,
    ITEM_SLATE,
    ITEM_WALL,
    ITEM_INVISIBLE_BORDER,
    ITEM_PORTAL,
    ITEM_SPAWN,
    ITEM_ENEMY_SPAWN,
    NUMBER_OF_ITEMS
};


class Inventory
{
public:
    Inventory(SDL_Setup* passedSdlSetup,
              std::list<Sprite*>& passedPlates,
              std::list<Sprite*>& passedSlates,
              std::list<Sprite*>& passedWalls,
              std::list<Sprite*>& passedInvisibleBorders,
              std::list<Portal*>& passedPortals,
              std::list<std::pair<int,int> >& passedEnemySpawns,
              int &passedSpawnX, int &passedSpawnY,
              int* passedCameraX, int* passedCameraY);
    ~Inventory();
    void Update();
    void DrawFront();
    void DrawBack();

    void PlaceNewWall(int x, int y, std::string filePath);
    void PlaceNewInvisibleBorder(int x, int y);
    void PlaceNewSlate(int x, int y, int w, int h, std::string filePath);
    void PlaceNewPlate(int x, int y, int w, int h, std::string filePath);
    void SetPlayerSpawn(int x, int y);
    void PlaceNewPortal(int x, int y, std::string filePath, std::string levelName);
    void PlaceNewEnemySpawn(int x, int y);

    bool& PlayerIsAllowedToMove(){return playerIsAllowedToMove;}
private:
    void DrawNewBlockSelection();
    void UpdateNewBlockSelection();

    std::list<Sprite*>& plates;
    std::list<Sprite*>& slates;
    std::list<Sprite*>& walls;
    std::list<Sprite*>& invisibleBorders;
    std::list<Portal*>& portals;
    TextField* portalLevel;

    std::list<std::pair<int,int> >& enemySpawns;
    Sprite* enemySpawnSprite;

    int& spawnX;
    int& spawnY;
    Sprite* playerSpawnSprite;
    Item* items[NUMBER_OF_ITEMS];
    int currentItem;
    TextField* spriteName;
    bool tileMode;
    Sprite* itemFrame;
    Sprite* currentItemFrame;

    //more like currentItemWidth
    int itemWidth;
    int itemHeight;

    int newBlockPosX;
    int newBlockPosY;
    bool drawNewBlockSelection;

    bool playerIsAllowedToMove;

    int* cameraX;
    int* cameraY;
    SDL_Setup* sdlSetup;
    int staticCamera;
};

#endif // INVENTORY_H
