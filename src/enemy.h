#ifndef ENEMY_H
#define ENEMY_H

#include "mob.h"
class MainCharacter;

class Enemy : public Mob
{
    public:
        Enemy(SDL_Setup* sdlSetup, int* cameraX, int* cameraY, int x, int y);
        Collision* GetCollisionBox(){return skin->GetCollisionBox();}
        static void SetPlayerPointer(MainCharacter* passedPlayer){player = passedPlayer;}
    protected:
        virtual void TakeInput();
    private:
        unsigned int randomDirTimeCheck;
        unsigned int fireFireballTimeCheck;

        static MainCharacter* player;
};

#endif // ENEMY_H
