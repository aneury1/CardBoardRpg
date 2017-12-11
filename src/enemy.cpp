#include "enemy.h"
#include <cstdlib>
#include <time.h>
#include <cmath>
#include "maincharacter.h"

MainCharacter* Enemy::player = NULL;

Enemy::Enemy(SDL_Setup* sdlSetup, int* cameraX, int* cameraY, int x, int y):
    Mob(sdlSetup,cameraX,cameraY,"enemy.png",x,y,58,85,DIR_DOWN,15,65,29,15,15,10,30,70,7)
{
    srand(time(NULL));

    randomDirTimeCheck = SDL_GetTicks();
    fireFireballTimeCheck = SDL_GetTicks();
}

void Enemy::TakeInput()
{
    if(DistanceBetween(GetCentreX(), GetDownY(), player->GetCentreX(), player->GetDownY()) > 300)//if enemy can't see the player
    {
        if(randomDirTimeCheck+350 < SDL_GetTicks())//move randomly
        {
            randomDirTimeCheck = SDL_GetTicks();
            if((rand()%5)==0)//stop randomly
            {
                randomDirTimeCheck +=400;
                return;
            }
            vel = 3;
            movementAngle = rand()%360;
        }
    }
    else
    {
        if(DistanceBetween(GetCentreX(), GetDownY(), player->GetCentreX(), player->GetDownY()) > 140)//keep distance
        {
            vel = 6;
        }
        else//don't walk, but turn to the player
        {
            vel = 0;

        }
        movementAngle = atan2((player->GetDownY()-GetDownY()),(player->GetCentreX()-GetCentreX())) * 180/M_PI +180;

        if(fireFireballTimeCheck + 500 < SDL_GetTicks())
        {
            fireFireballTimeCheck = SDL_GetTicks();
            FireFireball(1,player->GetCentreX(),player->GetCentreY(),"fireball_red.png");
        }
    }

    if(45<=movementAngle && movementAngle<135)
        faceDirection = DIR_UP;
    else if(135<=movementAngle && movementAngle<225)
        faceDirection = DIR_RIGHT;
    else if(225<=movementAngle && movementAngle<315)
        faceDirection = DIR_DOWN;
    else
        faceDirection = DIR_LEFT;
}

