#include "collision.h"

int Collision::newID = 0;

Collision::Collision(SDL_Renderer *passedRenderer, int *passedCameraX, int *passedCameraY, const int *passedX, const int *passedY)
{
    renderer = passedRenderer;

    collisionRect.x = 0;
    collisionRect.y = 0;
    collisionRect.w = 0;
    collisionRect.h = 0;

    cameraX = passedCameraX;
    cameraY = passedCameraY;

    x = passedX;
    y = passedY;

    id = newID;
    newID++;
}

bool Collision::IsColliding(Collision *object)
{
    /*return  !(((collisionRect.x + *x - collisionRect.w >= object->collisionRect.x + *object->x)||(collisionRect.x <= object->collisionRect.x + *object->x - object->collisionRect.w))
                                                                           &&
              ((collisionRect.y + *y - collisionRect.h >= object->collisionRect.y)||(collisionRect.y <= object->collisionRect.y + *object->y - object->collisionRect.h)));
*/
    if(id == object->id)
        return false;

    if( collisionRect.y + *y + collisionRect.h <= object->collisionRect.y + *object->y)
    {
        return false;
    }

    if( collisionRect.y + *y >= object->collisionRect.y + *object->y + object->collisionRect.h )
    {
        return false;
    }

    if( collisionRect.x + *x + collisionRect.w<= object->collisionRect.x + *object->x )
    {
        return false;
    }

    if( collisionRect.x + *x >= object->collisionRect.x + *object->x + object->collisionRect.w )
    {
        return false;
    }

    return true;
}

void Collision::Draw()
{
    if(!(collisionRect.w == 0 && collisionRect.h == 0))
    {
        SDL_SetRenderDrawColor(renderer,0xFF, 0x00, 0x00, 0xFF);
        SDL_Rect position;
        position.x = *x + collisionRect.x - *cameraX;
        position.y = *y + collisionRect.y - *cameraY;
        position.w = collisionRect.w;
        position.h = collisionRect.h;
        SDL_RenderDrawRect(renderer,&position);
        SDL_SetRenderDrawColor(renderer,0x00, 0x00, 0x00, 0x00);

    }
}

void Collision::SetCollisionRect(int x, int y, int w, int h)
{
    collisionRect.x = x;
    collisionRect.y = y;
    collisionRect.w = w;
    collisionRect.h = h;
}

