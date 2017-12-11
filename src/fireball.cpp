#include "fireball.h"
#include <cmath>

float DistanceBetween(int x1, int y1, int x2, int y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

Fireball::Fireball(int x, int y, int destX, int destY, std::string sprite, SDL_Renderer* renderer, int* cameraX, int* cameraY)
{
    initialX = x;
    initialY = y;

    texture = new Sprite(renderer,sprite,x,y,50,50,cameraX,cameraY);
    texture->SetUpAnimation(5,1);

    texture->SetCollisionBox(10,10,30,30);//hitbox

    float angle = atan2((destY-y),(destX-x)) * 180.0/M_PI +180.0;
    velY = (-10.0) * sin(DegreesToRadians(angle));
    velX = (-10.0) * cos(DegreesToRadians(angle));

    texture->Rotate(angle);
}

Fireball::~Fireball()
{
    delete texture;
}

void Fireball::Update()
{
    texture->SetX(texture->GetX()+velX);
    texture->SetY(texture->GetY()+velY);
    texture->PlayAnimation(0,4,0,50);
}

bool Fireball::TooFar()
{
    return (DistanceBetween(initialX,initialY,texture->GetX(),texture->GetY()) > 700);
}
