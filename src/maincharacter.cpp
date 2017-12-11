#include "maincharacter.h"

MainCharacter::MainCharacter(SDL_Setup *sdlSetup, int *passedCameraX, int *passedCameraY, Environment *environment, GameMode& passedGameMode) :
    Mob(sdlSetup,passedCameraX,passedCameraY,"player.png",0,0,58,85,DIR_DOWN,15,65,29,15,15,10,30,70,14), gameMode(passedGameMode)
{
    cameraX = passedCameraX;
    cameraY = passedCameraY;
    SetEnvironment(environment);

    moveUp = false;
    moveLeft = false;
    moveDown = false;
    moveUp = false;

    staticCamera = 0;
    healthbar = new Sprite(sdlSetup->GetRenderer(),"healthbar.png",220,370,200,20,&staticCamera,&staticCamera);
    healthbarIndicator = new Sprite(sdlSetup->GetRenderer(),"healthbar_indicator.png",222,372,196,16,&staticCamera,&staticCamera);
}

MainCharacter::~MainCharacter()
{
    delete healthbarIndicator;
    delete healthbar;
}

void MainCharacter::TakeInput()
{
    int mouseX = sdlSetup->GetMainEvent()->button.x+*cameraX;
    int mouseY = sdlSetup->GetMainEvent()->button.y+*cameraY;

    if(gameMode == GM_GAMEPLAY)
    {
        if(sdlSetup->MouseButtonLeftWasPressed())
        {
            FireFireball(3,mouseX,mouseY,"fireball_blue.png");
        }
    }

    if(sdlSetup->GetMainEvent()->type == SDL_MOUSEMOTION)
    {
        int angle = atan2((mouseY-(skin->GetY()+skin->GetHeight()/2)),(mouseX-(skin->GetX()+skin->GetWidth()/2))) * 180/M_PI +180;

        if(45<=angle && angle<135)
            faceDirection = DIR_UP;
        else if(135<=angle && angle<225)
            faceDirection = DIR_RIGHT;
        else if(225<=angle && angle<315)
            faceDirection = DIR_DOWN;
        else
            faceDirection = DIR_LEFT;
    }

    if(environment->PlayerIsAllowedToMove())
    {
        switch(sdlSetup->GetMainEvent()->type)
        {
        case SDL_KEYDOWN:
            vel = 8;
            switch(sdlSetup->GetMainEvent()->key.keysym.sym)
            {
            case SDLK_w:
                moveDown = false;
                moveUp = true;
                break;
            case SDLK_a:
                moveRight = false;
                moveLeft = true;
                break;
            case SDLK_s:
                moveUp = false;
                moveDown = true;
                break;
            case SDLK_d:
                moveLeft = false;
                moveRight = true;
                break;
            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch(sdlSetup->GetMainEvent()->key.keysym.sym)
            {
            case SDLK_w:
                moveUp = false;
                break;
            case SDLK_a:
                moveLeft = false;
                break;
            case SDLK_s:
                moveDown = false;
                break;
            case SDLK_d:
                moveRight = false;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }


        if(moveUp)
            movementAngle = 90;
        if(moveLeft)
            movementAngle = 0;
        if(moveDown)
            movementAngle = 270;
        if(moveRight)
            movementAngle = 180;

        if(!(moveUp || moveLeft || moveDown || moveRight))//all of them is false
        {
            vel = 0;
        }
        else
        {
            if(moveUp && moveLeft)
                movementAngle = 45;
            if(moveLeft && moveDown)
                movementAngle = 315;
            if(moveDown && moveRight)
                movementAngle = 225;
            if(moveRight && moveUp)
                movementAngle = 135;
        }
    }
}

void MainCharacter::TeleportTo(int x,int y)
{
    skin->SetX(x-skin->GetWidth()/2);
    skin->SetY(y-skin->GetHeight());
    *cameraX = skin->GetX()-320+skin->GetWidth()/2;
    *cameraY = skin->GetY()-200+skin->GetHeight()/2;
}

void MainCharacter::UpdateCamera()
{
    *cameraX = skin->GetX() - (320 - skin->GetWidth()/2);
    *cameraY = skin->GetY() - (200 - skin->GetHeight()/2);
}

void MainCharacter::DrawHealthBar()
{
    healthbar->Draw();

    if(GetHealth()>=0)
    {
        healthbarIndicator->SetWidth(14*GetHealth());
    }
    else
    {
        healthbarIndicator->SetWidth(0);
    }

    healthbarIndicator->Draw();
}
