#ifndef COLLISION_H
#define COLLISION_H

#include "rpg.h"

class Collision
{
public:
    Collision(SDL_Renderer* passedRenderer, int* passedCameraX, int* passedCameraY, const int *passedX, const int *passedY);

    bool IsColliding(Collision* object);
    void Draw();
    int GetX(){return *x;}
    int GetY(){return *y;}
    int GetID(){return id;}
private:
    //can't remember why i have made it private
    void SetCollisionRect(int x, int y, int w, int h);
    SDL_Renderer* renderer;
    SDL_Rect collisionRect;
    int *cameraX;
    int *cameraY;
    const int* x;
    const int* y;

    int id;
    static int newID;

    friend class Sprite;
    friend class Mob;
};

#endif // COLLISION_H
