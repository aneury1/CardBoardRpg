#ifndef SPRITE_H
#define SPRITE_H
#include "collision.h"
#include "drawable.h"

class Sprite : public Drawable
{
public:
    //can't pass filepath by reference, because passed string would dissapear after constructor call
    Sprite(SDL_Renderer* passedRenderer, std::string fileName, int x, int y, int width, int height, int* passedCameraX, int* passedCameraY);
    ~Sprite();
    void Draw();

    void LoadFromText(std::string text);
    void SetFontSize(int fontSize);
    void SetTextColor(int r,int g,int b,int a){textColor.r = r; textColor.g = g; textColor.b = b; textColor.a = a;}

    void SetX(int x){rect.x = x;}
    void SetY(int y){rect.y = y;}
    void SetWidth(int w){rect.w = w;}
    void SetCropping(int x, int y, int w, int h);
    void Rotate(int angle){rotationAngle = angle;}
    const int& GetX() const{return rect.x;}
    const int& GetY(){return rect.y;}
    int GetDownY(){return (rect.y + rect.h);}
    int GetWidth(){return rect.w;}
    int GetHeight(){return rect.h;}
    SDL_Rect GetRect(){return rect;}

    std::string GetTextureFileName(){return textureFileName;}

    //get image's real size in pixels
    int GetImgSizeW(){return imgSizeW;}
    int GetImgSizeH(){return imgSizeH;}
    void PlayAnimation(unsigned int beginFrame, unsigned int endFrame, unsigned int row, unsigned int delay);
    void SetUpAnimation(int AmountFramesX, int AmountFramesY);
    Collision* GetCollisionBox(){return collisionBox;}
    void SetCollisionBox(int x, int y, int w, int h);

    static bool CheckIfImageExists(std::string filePath, SDL_Renderer* passedRenderer);
private:
    SDL_Texture* texture;
    std::string textureFileName;
    int* cameraX;
    int* cameraY;
    SDL_Rect rect;
    SDL_Rect crop;
    int imgSizeW;
    int imgSizeH;
    int rotationAngle;

    SDL_Renderer* renderer;

    unsigned int currentFrame;
    unsigned int animationTimeCheck;

    unsigned int amountFramesX;
    unsigned int amountFramesY;

    Collision* collisionBox;
    TTF_Font* font;
    SDL_Color textColor;

    static const std::string SPRITE_FOLDER_PATH;
};

#endif // SPRITE_H
