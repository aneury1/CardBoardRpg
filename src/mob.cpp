#include "mob.h"
#include <cmath>

Environment* Mob::environment = NULL;
std::list<std::pair<Collision*,int> > Mob::hitboxes;
std::list<Collision*> Mob::collisionBoxes;

Mob::Mob(SDL_Setup *passedSdlSetup, int *passedCameraX, int *passedCameraY,
         std::string skinTextureName, int skinX, int skinY, int skinW, int skinH, Direction passedDirection,
         int collisionBoxX, int collisionBoxY, int collisionBoxW, int collisionBoxH,
         int hitboxX, int hitboxY, int hitboxW, int hitboxH, int health)
{
    sdlSetup = passedSdlSetup;

    cameraX = passedCameraX;
    cameraY = passedCameraY;

    faceDirection = passedDirection;
    skin = new Sprite(sdlSetup->GetRenderer(),skinTextureName
                        ,skinX,skinY,skinW, skinH,cameraX, cameraY);
    skin->SetCollisionBox(collisionBoxX,collisionBoxY,collisionBoxW,collisionBoxH);
    skin->SetUpAnimation(8,4);
    collisionBoxes.push_back(skin->GetCollisionBox());
    m_collisonBox = collisionBoxes.end();
    --m_collisonBox;

    vel = 0;
    velX = 0;
    velY = 0;
    movementAngle = 0;

    timeCheck = SDL_GetTicks();

    isColliding = false;

    hitboxes.push_back(std::make_pair(new Collision(sdlSetup->GetRenderer(),cameraX,cameraY,&skin->GetX(),&skin->GetY()),health));
    hitboxes.back().first->SetCollisionRect(hitboxX,hitboxY,hitboxW,hitboxH);
    m_hitbox = hitboxes.end();
    --m_hitbox;

    alive = true;
    dead = false;
    deathTime = 0;

    shootSound = new SoundEffect("shoot.wav",50);
}

Mob::~Mob()
{
    delete shootSound;

    for(std::list<Fireball*>::iterator i = fireballs.begin(); i!=fireballs.end(); ++i)
    {
        delete (*i);
    }
    fireballs.clear();

    Collision* temp = ((*m_hitbox).first);
    hitboxes.erase(m_hitbox);
    delete temp;

    collisionBoxes.erase(m_collisonBox);
    delete skin;
}

void Mob::Draw()
{
    if(faceDirection == DIR_UP || faceDirection == DIR_RIGHT)
        DrawFireballs();

    skin->Draw();
    if(faceDirection == DIR_DOWN || faceDirection == DIR_LEFT)
        DrawFireballs();
    #ifdef DEBUG
        m_hitbox->first->Draw();
    #endif
}

void Mob::Update()
{
    UpdateAnimation();
    UpdateMovement();
    UpdateFireballs();
}

void Mob::UpdateAnimation()
{
    if(alive)
    {
         if((vel == 0)||isColliding)
         {
             skin->PlayAnimation(0,0,int(faceDirection),0);
         }
         else
         {
             skin->PlayAnimation(0,3,int(faceDirection),130);
         }
    }
    else
    {
        skin->PlayAnimation(4,7,int(faceDirection),150);
    }
}

void Mob::UpdateMovement()
{
    if(alive)
        TakeInput();

    if(timeCheck + 15 < SDL_GetTicks() && vel!=0)
    {
        timeCheck = SDL_GetTicks();

        bool isCollidingY = false;
        bool isCollidingX = false;

        velY = (-1.0 * vel) * sin(DegreesToRadians(movementAngle));
        velX = (-1.0 * vel) * cos(DegreesToRadians(movementAngle));

        skin->SetY(skin->GetY() + velY);
        if(environment->IsColliding(skin->GetCollisionBox()) || IsCollidingWithMobs())
        {
            isCollidingY = true;
            skin->SetY(skin->GetY() - velY);
        }
        else
        {
            isCollidingY = false;
        }


        skin->SetX(skin->GetX() + velX);
        if(environment->IsColliding(skin->GetCollisionBox()) || IsCollidingWithMobs())
        {
            isCollidingX = true;
            skin->SetX(skin->GetX() - velX);
        }
        else
        {
            isCollidingX = false;
        }

        isColliding = isCollidingX||isCollidingY;
    }

    if(GetHealth()<=0 && alive && !dead)
    {
        alive = false;
        deathTime = SDL_GetTicks();
    }
    if(!alive && deathTime+700 <= SDL_GetTicks())
    {
        dead = true;
    }
}

bool Mob::AttackMob(Collision* attackArea, int damage, int attackerHitboxID)
{
    bool collided = false;

    for(std::list<std::pair<Collision*,int> >::iterator i = hitboxes.begin(), end = hitboxes.end(); i!=end; ++i)
    {
        if((*i).first->GetID()!= attackerHitboxID)
        {
            if(((*i).first)->IsColliding(attackArea))
            {
                (*i).second -=damage;
                collided = true;
            }
        }
    }
    return collided;
}

void Mob::FireFireball(int damage, int destX, int destY, std::string sprite)
{
    fireballDamage = damage;
    shootSound->Play();
    int x,y;
    y = GetY() - 10;
    switch(faceDirection)
    {
        case DIR_UP:
            x = GetX()-15;
            break;
        case DIR_LEFT:
        case DIR_RIGHT:
            x = GetX();
            break;
        case DIR_DOWN:
            x = GetX()+20;
            break;
        default:
            x = 0;
            break;
    }
    fireballs.push_back(new Fireball(x,y,destX,destY,sprite,sdlSetup->GetRenderer(),cameraX,cameraY));
}

void Mob::UpdateFireballs()
{
    for(std::list<Fireball*>::iterator i = fireballs.begin(); i!=fireballs.end();)
    {
        if(AttackMob((*i)->GetHitbox(),fireballDamage,m_hitbox->first->GetID()))
        {
            delete (*i);
            i = fireballs.erase(i);
        }
        else if(environment->IsColliding((*i)->GetHitbox()))
        {
            delete (*i);
            i = fireballs.erase(i);
        }
        else if((*i)->TooFar())
        {
            delete (*i);
            i = fireballs.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void Mob::DrawFireballs()
{
    for(std::list<Fireball*>::iterator i = fireballs.begin(); i!=fireballs.end(); ++i)
    {
        (*i)->Update();
        (*i)->Draw();
    }
}

bool Mob::IsCollidingWithMobs()
{
    for(std::list<Collision*>::iterator i = collisionBoxes.begin(), end = collisionBoxes.end(); i != end; ++i)
    {
        if((*i)->IsColliding(skin->GetCollisionBox()))
        {
            return true;
        }
    }
    return false;
}
