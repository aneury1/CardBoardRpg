#include "inventory.h"
#include <fstream>
#include "enemy.h"

const int INVENTORY_X = 585;
const int INVENTORY_Y = 10;

Inventory::Inventory(SDL_Setup *passedSdlSetup,
                     std::list<Sprite *> &passedPlates,
                     std::list<Sprite *> &passedSlates,
                     std::list<Sprite *> &passedWalls,
                     std::list<Sprite *> &passedInvisibleBorders,
                     std::list<Portal *> &passedPortals,
                     std::list<std::pair<int,int> >& passedEnemySpawns,
                     int &passedSpawnX, int &passedSpawnY,
                     int *passedCameraX, int *passedCameraY):
    plates(passedPlates),
    slates(passedSlates),
    walls(passedWalls),
    invisibleBorders(passedInvisibleBorders),
    portals(passedPortals),
    enemySpawns(passedEnemySpawns),
    spawnX(passedSpawnX),spawnY(passedSpawnY)
{
    cameraX = passedCameraX;
    cameraY = passedCameraY;
    staticCamera = 0;

    currentItem = 0;

    sdlSetup = passedSdlSetup;

    itemFrame = new Sprite(sdlSetup->GetRenderer(),"itemFrame.png",INVENTORY_X,INVENTORY_Y,22*2.5,142*2.5,&staticCamera,&staticCamera);
    itemFrame->SetCropping(0,0,22,142);
    currentItemFrame = new Sprite(sdlSetup->GetRenderer(),"itemFrame.png",INVENTORY_X-3,INVENTORY_Y-3,25*2.5,24*2.5,&staticCamera,&staticCamera);
    currentItemFrame->SetCropping(22,0,25,24);

    newBlockPosX = sdlSetup->GetMainEvent()->button.x;
    newBlockPosY = sdlSetup->GetMainEvent()->button.y;
    drawNewBlockSelection = false;

    items[ITEM_PLATE] = new Item("stone_brick",64,64,sdlSetup->GetRenderer(),2,INVENTORY_X,INVENTORY_Y);
    items[ITEM_SLATE] = new Item("leaves",64,64,sdlSetup->GetRenderer(),3,INVENTORY_X,INVENTORY_Y);
    items[ITEM_WALL] = new Item("wall",35,70,sdlSetup->GetRenderer(),0,INVENTORY_X,INVENTORY_Y);
    items[ITEM_INVISIBLE_BORDER] = new Item("outline",40,40,sdlSetup->GetRenderer(),1,INVENTORY_X,INVENTORY_Y);
    items[ITEM_PORTAL] = new Item("door_closed",70,90,sdlSetup->GetRenderer(),5,INVENTORY_X,INVENTORY_Y);
    portalLevel = new TextField(187,345,177,50,sdlSetup,"Portal's Level");
    portalLevel->Hide();

    items[ITEM_SPAWN] = new Item("player_spawner",35,57,sdlSetup->GetRenderer(),4,INVENTORY_X,INVENTORY_Y);
    playerSpawnSprite = new Sprite(sdlSetup->GetRenderer(),"player_spawner.png",0,0,35,57,cameraX,cameraY);//spawn position is currently unknown

    items[ITEM_ENEMY_SPAWN] = new Item("enemy_spawner",35,57,sdlSetup->GetRenderer(),6,INVENTORY_X,INVENTORY_Y);
    enemySpawnSprite = new Sprite(sdlSetup->GetRenderer(),"enemy_spawner.png",0,0,35,57,cameraX,cameraY);

    itemWidth = items[ITEM_PLATE]->GetDefaultW();
    itemHeight = items[ITEM_PLATE]->GetDefaultH();

    tileMode = false;

    spriteName = new TextField(5,345,177,50,sdlSetup,"Sprite Name");
    spriteName->SetText(items[currentItem]->GetTexturePath());

    playerIsAllowedToMove = true;
}

Inventory::~Inventory()
{
    for(int i=0;i<NUMBER_OF_ITEMS;++i)
    {
        delete items[i];
    }

    delete enemySpawnSprite;
    delete playerSpawnSprite;
    delete spriteName;
    delete portalLevel;
}


void Inventory::DrawFront()
{
    if(drawNewBlockSelection)
    {
        DrawNewBlockSelection();
    }


    itemFrame->Draw();
    for(int i=0; i<NUMBER_OF_ITEMS; ++i)
    {
        items[i]->DrawIcon();
    }
    currentItemFrame->Draw();

    spriteName->Draw();
    portalLevel->Draw();
}

void Inventory::DrawBack()
{
    playerSpawnSprite->Draw();
    for(std::list<std::pair<int,int> >::iterator i = enemySpawns.begin(), end = enemySpawns.end(); i!=end; ++i)
    {
        enemySpawnSprite->SetX(i->first);
        enemySpawnSprite->SetY(i->second);
        enemySpawnSprite->Draw();
    }
}

void Inventory::DrawNewBlockSelection()
{
    SDL_Rect tempRect;
    tempRect.x = newBlockPosX - *cameraX;
    tempRect.y = newBlockPosY - *cameraY;
    tempRect.w = items[currentItem]->GetDefaultW();
    tempRect.h = items[currentItem]->GetDefaultH();

    SDL_SetRenderDrawColor(sdlSetup->GetRenderer(),255,0,0,0);
    SDL_RenderDrawRect(sdlSetup->GetRenderer(),&tempRect);
    SDL_SetRenderDrawColor(sdlSetup->GetRenderer(),0,0,0,0);
}

void Inventory::Update()
{
    spriteName->Update();
    portalLevel->Update();
    if(spriteName->GetSelected() || portalLevel->GetSelected())
    {
        drawNewBlockSelection = false;
        playerIsAllowedToMove = false;
    }
    else
    {
        playerIsAllowedToMove = true;
    }

    if(!spriteName->GetSelected() && !portalLevel->GetSelected())
    {
        UpdateNewBlockSelection();

        if(currentItem != ITEM_ENEMY_SPAWN && currentItem != ITEM_SPAWN)
        {
            if(Sprite::CheckIfImageExists("to_use/" + spriteName->GetText() + ".png",sdlSetup->GetRenderer()))
            {
                items[currentItem]->SetTexturePath(spriteName->GetText());
            }
            else
            {
                sdlSetup->ShowMessage(spriteName->GetText() + ".png does not exist!");
                spriteName->SetText(items[currentItem]->GetTexturePath());
            }
        }

        int mouseX = sdlSetup->GetMainEvent()->button.x + *cameraX;
        int mouseY = sdlSetup->GetMainEvent()->button.y + *cameraY;

        if(sdlSetup->KeyWasPressed(SDL_SCANCODE_LCTRL))
        {
            tileMode = !tileMode;
            sdlSetup->ShowMessage("Tile Mode: " + to_string(tileMode));
        }

        if(sdlSetup->MouseButtonLeftWasPressed())
        {
            if((INVENTORY_X + *cameraX <= mouseX && mouseX <= INVENTORY_X+itemFrame->GetWidth() + *cameraX)
                                                 &&
               (INVENTORY_Y + *cameraY <= mouseY && mouseY <= INVENTORY_Y+itemFrame->GetHeight() + *cameraY))
            {
                for(int i=0;i<NUMBER_OF_ITEMS;++i)
                {
                    if(INVENTORY_Y+50*i+5 + *cameraY <= mouseY && mouseY < INVENTORY_Y+50*i+5+60 + *cameraY)
                    {
                        currentItem = i;
                        itemWidth = items[currentItem]->GetDefaultW();
                        itemHeight = items[currentItem]->GetDefaultH();
                        currentItemFrame->SetY(INVENTORY_Y+50*i-3);
                        spriteName->SetText(items[currentItem]->GetTexturePath());

                        if(currentItem == ITEM_SPAWN || currentItem == ITEM_INVISIBLE_BORDER || currentItem == ITEM_ENEMY_SPAWN)
                        {
                            spriteName->Hide();
                        }
                        else
                        {
                            spriteName->Unhide();
                        }

                        if(currentItem == ITEM_PORTAL)
                        {
                            portalLevel->Unhide();
                        }
                        else
                        {
                            portalLevel->Hide();
                        }
                    }
                }
            }
            else if(drawNewBlockSelection)
            {
                switch (currentItem) {
                case ITEM_WALL:
                    PlaceNewWall(newBlockPosX,newBlockPosY, "to_use/" + items[currentItem]->GetTexturePath() + ".png");
                    break;
                case ITEM_INVISIBLE_BORDER:
                    PlaceNewInvisibleBorder(newBlockPosX,newBlockPosY);
                    break;
                case ITEM_PLATE:
                    PlaceNewPlate(newBlockPosX,newBlockPosY,itemWidth,itemHeight, "to_use/" + items[currentItem]->GetTexturePath() + ".png");
                    break;
                case ITEM_SLATE:
                    PlaceNewSlate(newBlockPosX,newBlockPosY,itemWidth,itemHeight, "to_use/" + items[currentItem]->GetTexturePath() + ".png");
                    break;
                case ITEM_SPAWN:
                    SetPlayerSpawn(newBlockPosX+playerSpawnSprite->GetWidth()/2,newBlockPosY+playerSpawnSprite->GetHeight());
                    break;
                case ITEM_PORTAL:
                    if(CheckLevelForExistance(portalLevel->GetText()))
                    {
                        PlaceNewPortal(newBlockPosX,newBlockPosY, "to_use/" + items[currentItem]->GetTexturePath() + ".png", portalLevel->GetText());
                    }
                    else
                    {
                        sdlSetup->ShowMessage("Can't find level '" + portalLevel->GetText() + "'!");
                        portalLevel->SetText("");
                    }
                    break;
                case ITEM_ENEMY_SPAWN:
                    PlaceNewEnemySpawn(newBlockPosX,newBlockPosY);
                    break;
                default:
                    break;
                }
            }
        }

        if(sdlSetup->KeyWasPressed(SDL_SCANCODE_DELETE))
        {
            switch (currentItem)
            {
            case ITEM_WALL:
                if(!walls.empty())
                {
                    delete walls.back();
                    walls.pop_back();
                }
                break;
            case ITEM_INVISIBLE_BORDER:
                if(!invisibleBorders.empty())
                {
                    delete invisibleBorders.back();
                    invisibleBorders.pop_back();
                }
                break;
            case ITEM_PLATE:
                if(!plates.empty())
                {
                    delete plates.back();
                    plates.pop_back();
                }
                break;
            case ITEM_SLATE:
                if(!slates.empty())
                {
                    delete slates.back();
                    slates.pop_back();
                }
                break;
            case ITEM_PORTAL:
                if(!portals.empty())
                {
                    delete portals.back();
                    portals.pop_back();
                }
            case ITEM_ENEMY_SPAWN:
                if(!enemySpawns.empty())
                {
                    enemySpawns.pop_back();
                }
            default:
                break;
            }
        }
    }
}

void Inventory::PlaceNewWall(int x, int y, std::string filePath)
{
    walls.push_back(new Sprite(sdlSetup->GetRenderer(),filePath,x,y,items[ITEM_WALL]->GetDefaultW(),items[ITEM_WALL]->GetDefaultH(),cameraX,cameraY));
    walls.back()->SetCollisionBox(3,50,32,20);
}

void Inventory::PlaceNewInvisibleBorder(int x, int y)
{
    invisibleBorders.push_back(new Sprite(sdlSetup->GetRenderer(),"to_use/" + items[ITEM_INVISIBLE_BORDER]->GetTexturePath() + ".png",x,y,items[ITEM_INVISIBLE_BORDER]->GetDefaultW(),items[ITEM_INVISIBLE_BORDER]->GetDefaultH(),cameraX,cameraY));
    invisibleBorders.back()->SetCollisionBox(0,0,40,40);
}

void Inventory::PlaceNewPlate(int x, int y, int w, int h, std::string filePath)
{
    plates.push_back(new Sprite(sdlSetup->GetRenderer(),filePath,x,y,w,h,cameraX,cameraY));
}

void Inventory::PlaceNewSlate(int x, int y, int w, int h, std::string filePath)
{
    slates.push_back(new Sprite(sdlSetup->GetRenderer(),filePath,x,y,w,h,cameraX,cameraY));
}

void Inventory::SetPlayerSpawn(int x, int y)
{
    playerSpawnSprite->SetX(x-playerSpawnSprite->GetWidth()/2);
    playerSpawnSprite->SetY(y-playerSpawnSprite->GetHeight());
    spawnX = x;
    spawnY = y;
}

void Inventory::PlaceNewPortal(int x, int y, std::string filePath, std::string levelName)
{
    portals.push_back(new Portal(x,y,levelName,filePath,sdlSetup->GetRenderer(),cameraX,cameraY));
}

void Inventory::PlaceNewEnemySpawn(int x, int y)
{
    enemySpawns.push_back(std::make_pair(x,y));
}

void Inventory::UpdateNewBlockSelection()
{
    int mouseX = sdlSetup->GetMainEvent()->button.x + *cameraX;
    int mouseY = sdlSetup->GetMainEvent()->button.y + *cameraY;


    if(sdlSetup->GetMainEvent()->type == SDL_MOUSEMOTION)
    {
        if(tileMode)
        {
            newBlockPosX = mouseX - (mouseX%items[currentItem]->GetDefaultW());
            newBlockPosY = mouseY - (mouseY%items[currentItem]->GetDefaultH());
            if(mouseX<0)
            {
                newBlockPosX -= items[currentItem]->GetDefaultW();
            }
            if(mouseY<0)
            {
                newBlockPosY -= items[currentItem]->GetDefaultH();
            }
        }
        else
        {
            newBlockPosX = mouseX - items[currentItem]->GetDefaultW()/2;
            newBlockPosY = mouseY - items[currentItem]->GetDefaultH()/2;
        }
        drawNewBlockSelection = true;
    }
    else if(sdlSetup->GetMainEvent()->type == SDL_KEYDOWN)
    {
        drawNewBlockSelection = false;
    }
}
