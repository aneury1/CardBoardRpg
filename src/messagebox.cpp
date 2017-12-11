#include "messagebox.h"

MessageBox::MessageBox(SDL_Renderer* renderer)
{
    staticCamera = 0;

    messageBoxSprite = new Sprite(renderer,"messageBox.png",-3,-51,370,50,&staticCamera,&staticCamera);
    textSprite = new Sprite(renderer,"error.png",messageBoxSprite->GetX()+10,messageBoxSprite->GetY()+15,100,30,&staticCamera,&staticCamera);
    textSprite->SetFontSize(20);

    timeCheck = 0;
    timeChecked = false;
}

MessageBox::~MessageBox()
{
    /*delete textSprite;
    delete messageBoxSprite;*/
}

void MessageBox::Draw()
{
    if(!textMessages.empty())
    {
        messageBoxSprite->Draw();
        textSprite->Draw();
    }
}

void MessageBox::Update()
{
    if(!textMessages.empty())
    {
        textSprite->LoadFromText(textMessages.front());
        const unsigned int popUpTime = 300;
        const unsigned int duration = 2000;
        const unsigned int disappearTime = popUpTime;
        if(!timeChecked)
        {
            timeCheck = SDL_GetTicks();
            timeChecked = true;
        }
        if(SDL_GetTicks() < timeCheck + popUpTime)//pop up
        {
            messageBoxSprite->SetY(messageBoxSprite->GetY()+2);
        }
        else if((SDL_GetTicks() >= timeCheck + (popUpTime + duration + disappearTime)) && timeChecked)
        {
            messageBoxSprite->SetY(-51);
            timeChecked = false;
            textMessages.pop();
        }
        else if(SDL_GetTicks() >= timeCheck + (popUpTime + duration))//disappear
        {
            messageBoxSprite->SetY(messageBoxSprite->GetY()-2);
        }

        textSprite->SetY(messageBoxSprite->GetY()+15);
    }
}

void MessageBox::PopUp(std::string message)
{
    textMessages.push(message);
    DebugCOut("MessageBox: " + textMessages.back()); //if MessageBox would not work correctly
}
