#include "portal.h"

Portal::Portal(int x, int y, std::string levelName, std::string spriteName, SDL_Renderer* renderer, int* cameraX, int* cameraY) :
    Sprite(renderer,spriteName,x,y,70,90,cameraX,cameraY),
    levelName(levelName)
{}

bool Portal::PlayerIsInPortal(SDL_Rect playerRect)
{
    if((GetRect().x <= playerRect.x) && (playerRect.x + playerRect.w <= GetRect().x + GetRect().w) &&
      (GetRect().y <= playerRect.y) && (playerRect.y + playerRect.h <= GetRect().y + GetRect().h))
    {
        return true;
    }
    return false;
}
