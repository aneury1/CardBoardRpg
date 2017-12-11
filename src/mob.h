#ifndef MOB_H
#define MOB_H
#include "environment.h"
#include "drawable.h"
#include <list>
#include "fireball.h"
#include "soundeffect.h"

enum Direction
{
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_TOTAL
};

class Mob : public Drawable
{
public:
    Mob(SDL_Setup *passedSdlSetup, int *passedCameraX, int *passedCameraY,std::string skinTextureName,
        int skinX, int skinY, int skinW, int skinH, Direction passedDirection,
        int collisionBoxX, int collisionBoxY, int collisionBoxW, int collisionBoxH,
        int hitboxX, int hitboxY, int hitboxW, int hitboxH, int health);
    ~Mob();
    void Update();
    void Draw();
    static void SetEnvironment( Environment *passedEnvironment){environment = passedEnvironment;}
    int GetX(){return skin->GetX();}
    int GetY(){return skin->GetY();}
    int GetCentreX(){return skin->GetX()+skin->GetWidth()/2;}
    int GetCentreY(){return skin->GetY()+skin->GetHeight()/2;}
    int GetDownY(){return skin->GetDownY();}
    SDL_Rect GetRect(){return skin->GetRect();}
    bool Dead(){return dead;}
    int GetHealth(){return m_hitbox->second;}
protected:
    virtual void TakeInput()=0;
    bool AttackMob(Collision* attackArea, int damage, int attackerHitboxID);
    void FireFireball(int damage, int destX, int destY, std::string sprite);
    bool IsCollidingWithMobs();

    SDL_Setup* sdlSetup;
    static Environment* environment;
    Sprite* skin;

    int vel;
    int movementAngle;

    Direction faceDirection;
private:
    void UpdateMovement();
    void UpdateAnimation();
    void UpdateFireballs();
    void DrawFireballs();

    int velX;
    int velY;
    bool isColliding;
    unsigned int timeCheck;

    std::list<Fireball*> fireballs;
    int fireballDamage;
    SoundEffect* shootSound;

    static std::list<std::pair<Collision*,int> > hitboxes;//to check whether mob hits others
    std::list<std::pair<Collision*,int> >::iterator m_hitbox;

    static std::list<Collision*> collisionBoxes;// to check whether mobs collides with others
    std::list<Collision*>::iterator m_collisonBox;

    unsigned int deathTime;
    bool alive;//there is an interval between being dead and alive
    bool dead;

    int* cameraX;
    int* cameraY;
};

#endif // MOB_H
