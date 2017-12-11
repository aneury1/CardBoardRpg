#include "item.h"

int Item::count = 0;
Item::Item(std::string defaultTexturePath, int DefaultW, int DefaultH, SDL_Renderer* renderer, int iconCrop, const int passed_INVENTORY_X, const int passed_INVENTORY_Y):INVENTORY_X(passed_INVENTORY_X),INVENTORY_Y(passed_INVENTORY_Y)
{
    defaultW = DefaultW;
    defaultH = DefaultH;
    if(defaultW == 0)
    {
        DebugCOut("Warning: Item::defaultW == 0!!!");
    }
    if(defaultH == 0)
    {
        DebugCOut("Warning: Item::defaultH == 0!!!");
    }

    staticCamera = 0;

    icon = new Sprite(renderer,"icons.png",INVENTORY_X+8,INVENTORY_Y+50*count+8,40,39,&staticCamera,&staticCamera);
    icon->SetCropping(32*iconCrop,0,32,32);
    texturePath = defaultTexturePath;

    count++;
}

void Item::SetTexturePath(std::string newTexturePath)
{
    texturePath = newTexturePath;
}

void Item::DrawIcon()
{
    icon->Draw();
}

Item::~Item()
{
    delete icon;
}

