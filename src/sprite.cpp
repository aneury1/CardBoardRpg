#include "sprite.h"
#include <cstdlib>

const std::string Sprite::SPRITE_FOLDER_PATH = "data/sprites/";//static member

Sprite::Sprite(SDL_Renderer* passedRenderer, std::string fileName, int x, int y, int width, int height, int* passedCameraX, int* passedCameraY)
{
    renderer = passedRenderer;
    texture = NULL;

    textureFileName = fileName;
    fileName = SPRITE_FOLDER_PATH + fileName;
    texture = IMG_LoadTexture(renderer,fileName.c_str());
    if(texture == NULL)
    {
        DebugCOut("Can't load " + fileName);
    }


    SDL_QueryTexture(texture,NULL,NULL,&imgSizeW,&imgSizeH);
    crop.x = 0;
    crop.y = 0;
    crop.w = imgSizeW;
    crop.h = imgSizeH;

    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    rotationAngle = 0;

    cameraX = passedCameraX;
    cameraY = passedCameraY;

    currentFrame = -1;//if i put 0, animation would not be played the first time
    animationTimeCheck = SDL_GetTicks();

    collisionBox = NULL;

    font = TTF_OpenFont("data/agency fb.ttf",14);
    if(font == NULL)
    {
        DebugCOut("Can't load data/agency fb.ttf");
    }
    TTF_SetFontStyle(font,TTF_STYLE_BOLD);

    textColor.r = 240;
    textColor.g = 240;
    textColor.b = 240;
    textColor.a = 0;
}

Sprite::~Sprite()
{
    SDL_DestroyTexture(texture);

    if(collisionBox!=NULL)
    {
        delete collisionBox;
    }

    TTF_CloseFont(font);
}

void Sprite::Draw()
{
    SDL_Rect tempRect;
    tempRect = rect;
    tempRect.x -= *cameraX;
    tempRect.y -= *cameraY;
    SDL_RenderCopyEx(renderer,texture,&crop,&tempRect,rotationAngle,NULL,SDL_FLIP_NONE);

    #ifdef DEBUG
        if(collisionBox!=NULL)
            collisionBox->Draw();
    #endif // DEBUG
}

void Sprite::LoadFromText(std::string text)
{
    SDL_DestroyTexture(texture);

    text += " ";
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), textColor );
    if( textSurface == NULL )
    {
        DebugCOut("Can't render text: " + text + SDL_GetError());
    }
    else
    {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == NULL )
        {
            DebugCOut("Can't create texture from rendered text: " + text);
        }
        else
        {
            //Get image dimensions
            rect.w = textSurface->w;
            rect.h = textSurface->h;
            SDL_QueryTexture(texture,NULL,NULL,&imgSizeW,&imgSizeH);
            crop.w = imgSizeW;
            crop.h = imgSizeH;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
}

void Sprite::SetFontSize(int fontSize)
{
    TTF_CloseFont(font);
    font = TTF_OpenFont("data/agency fb.ttf",fontSize);
    if(font == NULL)
    {
        DebugCOut("Can't load data/agency fb.ttf. Font size: " + to_string<int>(fontSize));
    }
    TTF_SetFontStyle(font,TTF_STYLE_BOLD);
}

void Sprite::SetUpAnimation(int AmountFramesX, int AmountFramesY)
{
    amountFramesX = AmountFramesX;
    amountFramesY = AmountFramesY;
    crop.x = 0;
    crop.y = 0;
    crop.w = imgSizeW/amountFramesX;
    crop.h = imgSizeH/amountFramesY;
}

void Sprite::PlayAnimation(unsigned int beginFrame, unsigned int endFrame, unsigned int row, unsigned int delay)
{
    if(!((beginFrame == endFrame) && (endFrame == currentFrame)))//the same frame
    {
        if((animationTimeCheck + delay) < SDL_GetTicks())
        {
            if(currentFrame > endFrame)
                currentFrame = beginFrame;

            crop.x = imgSizeW/amountFramesX * currentFrame;
            crop.y = imgSizeH/amountFramesY * row;
            crop.w = imgSizeW/amountFramesX;
            crop.h = imgSizeH/amountFramesY;

            currentFrame++;
            animationTimeCheck = SDL_GetTicks();
        }
    }
}

void Sprite::SetCropping(int x, int y, int w, int h)
{
    crop.x = x;
    crop.y = y;
    crop.w = w;
    crop.h = h;
}

void Sprite::SetCollisionBox(int x, int y, int w, int h)
{
    if(collisionBox == NULL)
    {
        collisionBox = new Collision(renderer,cameraX,cameraY,&rect.x,&rect.y);
    }
    collisionBox->SetCollisionRect(x,y,w,h);
}

bool Sprite::CheckIfImageExists(std::string filePath, SDL_Renderer* passedRenderer)
{
    filePath = SPRITE_FOLDER_PATH + filePath;
    SDL_Texture* tempTexture = IMG_LoadTexture(passedRenderer,filePath.c_str());
    if(tempTexture == NULL)
    {
        return false;
    }
    SDL_DestroyTexture(tempTexture);
    return true;
}
