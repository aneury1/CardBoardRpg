#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include "mob.h"

class MainCharacter : public Mob
{
public:
    MainCharacter(SDL_Setup* sdlSetup, int* passedCameraX, int* passedCameraY, Environment* environment, GameMode& passedGameMode);
    ~MainCharacter();
    void TeleportTo(int x,int y);
    void UpdateCamera();
    void DrawHealthBar();
protected:
    virtual void TakeInput();
private:
    int* cameraX;
    int* cameraY;

    bool moveUp;
    bool moveLeft;
    bool moveDown;
    bool moveRight;

    Sprite* healthbar;
    Sprite* healthbarIndicator;
    int staticCamera;

    GameMode& gameMode;
};

#endif // MAINCHARACTER_H
