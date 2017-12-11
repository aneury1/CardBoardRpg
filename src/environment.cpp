#include "environment.h"
#include <algorithm>
#include "enemy.h"

bool compareDrawablesByY(Drawable *a, Drawable *b)
{
    return a->GetDownY() < b->GetDownY();
}


Environment::Environment(SDL_Setup* passedSdlSetup, int *passedCameraX, int *passedCameraY)
{
    cameraX = passedCameraX;
    cameraY = passedCameraY;

    sdlSetup = passedSdlSetup;

    inventory = new Inventory(sdlSetup,plates,slates,walls,invisibleBorders,portals,enemySpawns,spawnX,spawnY,cameraX,cameraY);

    currentMap = "";

    spawnX = 0;
    spawnY = 0;
}

Environment::~Environment()
{
    DeleteEnvironment();

    delete inventory;
}

void Environment::DrawBack(int playerY)
{
    for(std::list<Sprite*>::iterator i = plates.begin(), end = plates.end(); i != end; ++i)
    {
        (*i)->Draw();
    }

    if(gameMode == GM_LEVELEDIT)
    {
        inventory->DrawBack();
    }

    if(gameMode == GM_LEVELEDIT)
    {
        for(std::list<Sprite*>::iterator i = invisibleBorders.begin(), end = invisibleBorders.end(); i != end; ++i)
        {
            (*i)->Draw();
        }
    }

    toDrawFrontBegin = toDraw.begin();
    for(std::list<Drawable*>::iterator i = toDraw.begin(), end = toDraw.end(); i != end; ++i)
    {
        if((*i)->GetDownY() <= playerY)
        {
            (*i)->Draw();
        }
    }
}

void Environment::DrawFront(int playerY)
{
    for(std::list<Drawable*>::iterator i = toDrawFrontBegin, end = toDraw.end(); i != end; ++i)
    {
        if((*i)->GetDownY() > playerY)
        {
            (*i)->Draw();
        }
    }

    for(std::list<Sprite*>::iterator i = slates.begin(), end = slates.end(); i != end; ++i)
    {
        (*i)->Draw();
    }
}

void Environment::DrawInvetory()
{
    inventory->DrawFront();
}

void Environment::Update()
{
    if(gameMode == GM_LEVELEDIT)
    {
        inventory->Update();
    }

    toDraw.clear();
    toDraw.insert(toDraw.end(),walls.begin(),walls.end());
    toDraw.insert(toDraw.end(),portals.begin(),portals.end());
    if(gameMode == GM_GAMEPLAY)
    {
        for(std::list<Enemy*>::iterator i = enemies.begin(); i != enemies.end();)
        {
            (*i)->Update();
            if((*i)->Dead())
            {
                Enemy* temp = (*i);
                i = enemies.erase(i);
                delete temp;
            }
            else
            {
                ++i;
            }
        }
        toDraw.insert(toDraw.end(),enemies.begin(),enemies.end());
    }
    toDraw.sort(compareDrawablesByY);
}


void Environment::SaveToFile()
{
    std::string levelPath = "data/maps/" + currentMap + ".txt";
    std::ofstream levelFile(levelPath.c_str());

    levelFile << "SPAWN: " << spawnX << ' ' << spawnY << " ;" << std::endl;//it's a kind of magic

    levelFile << "WALLS: ";
    for(std::list<Sprite*>::iterator i = walls.begin(), end = walls.end(); i != end; ++i)
    {
        levelFile << (*i)->GetTextureFileName() << ' ' << (*i)->GetX() << ' ' << (*i)->GetY() << ' ';
    }
    levelFile << ';' << std::endl;

    levelFile << "INVISIBLE_WALLS: ";
    for(std::list<Sprite*>::iterator i = invisibleBorders.begin(), end = invisibleBorders.end(); i != end; ++i)
    {
        levelFile << (*i)->GetX() << ' ' << (*i)->GetY() << ' ';
    }
    levelFile << ';' << std::endl;

    levelFile << "PLATES: ";
    for(std::list<Sprite*>::iterator i = plates.begin(), end = plates.end(); i != end; ++i)
    {
        levelFile << (*i)->GetTextureFileName() << ' ' << (*i)->GetX() << ' ' << (*i)->GetY() << ' ' << (*i)->GetWidth() << ' ' << (*i)->GetHeight() << ' ';
    }
    levelFile << ';' << std::endl;

    levelFile << "SLATES: ";
    for(std::list<Sprite*>::iterator i = slates.begin(), end = slates.end(); i != end; ++i)
    {
        levelFile << (*i)->GetTextureFileName() << ' ' << (*i)->GetX() << ' ' << (*i)->GetY() << ' ' << (*i)->GetWidth() << ' ' << (*i)->GetHeight() << ' ';
    }
    levelFile << ';' << std::endl;

    levelFile << "PORTALS: ";
    for(std::list<Portal*>::iterator i = portals.begin(), end = portals.end(); i != end; ++i)
    {
        levelFile << (*i)->GetTextureFileName() << ' ' << (*i)->GetX() << ' ' << (*i)->GetY() << ' ' << (*i)->levelName << ' ';
    }
    levelFile << ';' << std::endl;

    levelFile << "ENEMY_SPAWNS: ";
    for(std::list<std::pair<int,int> >::iterator i = enemySpawns.begin(), end = enemySpawns.end(); i != end; ++i)
    {
        levelFile << i->first << ' ' << i->second << ' ';
    }
    levelFile << ';' << std::endl;

    levelFile << "END";//End
    levelFile.close();

    DebugCOut("level '" + currentMap + "' saved!");
}



bool Environment::LoadFromFile(std::string levelName, GameMode passedGameMode)
{
    gameMode = passedGameMode;

    std::string levelPath = "data/maps/" + levelName + ".txt";
    std::ifstream levelFile(levelPath.c_str());
    if(levelFile.is_open())
    {
        DeleteEnvironment();

        int x, y, w, h;
        std::string type, textureFileName;
        char semicolon;
        levelFile >> type >> x >> y >> semicolon;
        inventory->SetPlayerSpawn(x,y);
        while (levelFile.good())
        {
            levelFile >> type;
            if(type == "END")
            {
                toDraw.clear();
                toDraw.insert(toDraw.end(),walls.begin(),walls.end());
                toDraw.insert(toDraw.end(),portals.begin(),portals.end());

                if(gameMode == GM_GAMEPLAY)
                {
                    SpawnEnemies();
                    toDraw.insert(toDraw.end(),enemies.begin(),enemies.end());
                }
                toDraw.sort(compareDrawablesByY);

                DebugCOut("level '" + levelName + "' loaded!");

                currentMap = levelName;
                return true;
            }
            while(true)
            {
                levelFile >> semicolon;
                if(semicolon != ';')
                {
                    levelFile.putback(semicolon);

                    if(type == "WALLS:")
                    {
                        levelFile >> textureFileName >> x >> y;
                        inventory->PlaceNewWall(x,y,textureFileName);
                    }
                    if(type == "INVISIBLE_WALLS:")
                    {
                        levelFile >> x >> y;
                        inventory->PlaceNewInvisibleBorder(x,y);
                    }
                    if(type == "PLATES:")
                    {
                        levelFile >> textureFileName >> x >> y >> w >> h;
                        inventory->PlaceNewPlate(x,y,w,h,textureFileName);
                    }
                    if(type == "SLATES:")
                    {
                        levelFile >> textureFileName >> x >> y >> w >> h;
                        inventory->PlaceNewSlate(x,y,w,h,textureFileName);
                    }
                    if(type == "PORTALS:")
                    {
                        static std::string levelName;
                        levelFile >> textureFileName >> x >> y >> levelName;
                        inventory->PlaceNewPortal(x,y,textureFileName,levelName);
                    }
                    if(type == "ENEMY_SPAWNS:")
                    {
                        levelFile >> x >> y;
                        inventory->PlaceNewEnemySpawn(x,y);
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    else
    {
        sdlSetup->ShowMessage("Can't open level '" + levelName + "'!");
    }
    return false;
}

bool Environment::IsColliding(Collision *object)
{
    bool isColliding = false;

    for(std::list<Sprite*>::iterator i = walls.begin(), end = walls.end(); i != end; ++i)
    {
        if((*i)->GetCollisionBox()->IsColliding(object))
        {
            isColliding = true;
        }
    }

    for(std::list<Sprite*>::iterator i = invisibleBorders.begin(), end = invisibleBorders.end(); i != end; ++i)
    {
        if((*i)->GetCollisionBox()->IsColliding(object))
        {
            isColliding = true;
        }
    }

    return isColliding;
}

void Environment::DeleteEnvironment()
{
    if(currentMap != "")
    {
        SaveToFile();
    }

    for(std::list<Sprite*>::iterator i = walls.begin(), end = walls.end(); i != end; ++i)
    {
        delete (*i);
    }
    walls.clear();

    for(std::list<Sprite*>::iterator i = plates.begin(), end = plates.end(); i != end; ++i)
    {
        delete (*i);
    }
    plates.clear();

    for(std::list<Sprite*>::iterator i = slates.begin(), end = slates.end(); i != end; ++i)
    {
        delete (*i);
    }
    slates.clear();

    for(std::list<Sprite*>::iterator i = invisibleBorders.begin(), end = invisibleBorders.end(); i != end; ++i)
    {
        delete (*i);
    }
    invisibleBorders.clear();

    for(std::list<Portal*>::iterator i = portals.begin(), end = portals.end(); i != end; ++i)
    {
        delete (*i);
    }
    portals.clear();

    if(!enemies.empty())
    {
        for(std::list<Enemy*>::iterator i = enemies.begin(), end = enemies.end(); i != end; ++i)
        {
            delete (*i);
        }
        enemies.clear();
    }
    toDraw.clear();


    enemySpawns.clear();
}

bool Environment::PlayerIsInPortal(SDL_Rect playerRect)
{
    for(std::list<Portal*>::iterator i = portals.begin(), end = portals.end(); i != end; ++i)
    {
        if((*i)->PlayerIsInPortal(playerRect))
        {
            portalThatThePlayerIsIn = i;
            return true;
        }
    }
    return false;
}

void Environment::SpawnEnemies()
{
    for(std::list<std::pair<int,int> >::iterator i = enemySpawns.begin(), end = enemySpawns.end(); i != end; ++i)
    {
        enemies.push_back(new Enemy(sdlSetup,cameraX,cameraY,i->first,i->second));
    }
}




