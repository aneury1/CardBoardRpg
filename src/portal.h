#ifndef PORTAL_H
#define PORTAL_H

#include "sprite.h"

class Portal : public Sprite
{
    public:
        Portal(int x, int y, std::string levelName, std::string spriteName, SDL_Renderer* renderer, int* cameraX, int* cameraY);
        bool PlayerIsInPortal(SDL_Rect playerRect);
        const std::string levelName;//to which this portal is going to transfer
};

#endif // PORTAL_H
