#ifndef FIREBALL_H
#define FIREBALL_H
#include "sprite.h"

float DistanceBetween(int x1, int y1, int x2, int y2);

class Fireball
{
    public:
        Fireball(int x, int y, int destX, int destY, std::string sprite, SDL_Renderer* renderer, int* cameraX, int* cameraY);
        void Update();
        void Draw(){texture->Draw();}
        Collision* GetHitbox(){return texture->GetCollisionBox();}
        bool TooFar();
        ~Fireball();
    private:
        Sprite* texture;
        int velX;
        int velY;

        int initialX;
        int initialY;
};

#endif // FIREBALL_H
