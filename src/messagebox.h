#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "sprite.h"
#include <queue>

class MessageBox
{
    public:
        MessageBox(SDL_Renderer* renderer);
        ~MessageBox();
        void PopUp(std::string message);
        void Draw();
        void Update();
    private:

        Sprite* messageBoxSprite;

        Sprite* textSprite;
        std::queue<std::string> textMessages;

        unsigned int timeCheck;
        bool timeChecked;

        int staticCamera;
};

#endif // MESSAGEBOX_H
