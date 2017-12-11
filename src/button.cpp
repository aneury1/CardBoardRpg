#include "button.h"

Button::Button(SDL_Setup* passedSdlSetup, int x, int y, std::string buttonText)
{
    sdlSetup = passedSdlSetup;
    staticCamera = 0;

    text = buttonText;
    textSprite = new Sprite(sdlSetup->GetRenderer(),"error.png",x+10,y+7,250,40,&staticCamera,&staticCamera);
    textSprite->SetFontSize(20);
    textSprite->LoadFromText(text);

    buttonState = BS_UNCLICKED;

    buttonTextureClicked = new Sprite(sdlSetup->GetRenderer(),"button.png",x,y,250,40,&staticCamera,&staticCamera);
    buttonTextureClicked->SetCropping(0,20*0,117,20);
    buttonTextureUnclicked = new Sprite(sdlSetup->GetRenderer(),"button.png",x,y,250,40,&staticCamera,&staticCamera);
    buttonTextureUnclicked->SetCropping(0,20*1,117,20);
    buttonTextureMouseOver = new Sprite(sdlSetup->GetRenderer(),"button.png",x,y,250,40,&staticCamera,&staticCamera);
    buttonTextureMouseOver->SetCropping(0,20*2,117,20);

    wasClicked = false;

    clickSound = new SoundEffect("click.wav",40);
}

Button::~Button()
{
    delete clickSound;
    delete buttonTextureClicked;
    delete buttonTextureMouseOver;
    delete buttonTextureUnclicked;
}

void Button::Update()
{
    int mouseX = sdlSetup->GetMainEvent()->button.x;
    int mouseY = sdlSetup->GetMainEvent()->button.y;

    if((buttonTextureClicked->GetX() <= mouseX && mouseX <= buttonTextureClicked->GetX() + buttonTextureClicked->GetWidth())
                                                 &&
    (buttonTextureClicked->GetY() <= mouseY && mouseY <= buttonTextureClicked->GetY() + buttonTextureClicked->GetHeight()))
    {
        if(sdlSetup->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN)
        {
            if(sdlSetup->GetMainEvent()->button.button == SDL_BUTTON_LEFT)
            {
                buttonState = BS_CLICKED;
            }
        }
        else
        {
            if(buttonState == BS_CLICKED)//was clicked, but now it is not
            {
                wasClicked = true;
                clickSound->Play();
            }
            if(buttonState == BS_MOUSE_OVER)
            {
                wasClicked = false;
            }
            buttonState = BS_MOUSE_OVER;
        }
    }
    else
    {
        buttonState = BS_UNCLICKED;
        wasClicked = false;
    }
}

void Button::Draw()
{
    switch(buttonState)
    {
    case BS_CLICKED:
        textSprite->SetTextColor(100,100,100,0);//gray
        textSprite->LoadFromText(text);
        buttonTextureClicked->Draw();
        break;
    case BS_MOUSE_OVER:
        /*textSprite->SetTextColor(240,240,0,0);//yellow
        textSprite->LoadFromText(text);*/
        buttonTextureMouseOver->Draw();
        break;
    case BS_UNCLICKED:
        buttonTextureUnclicked->Draw();
        break;
    default:
        break;
    }
    textSprite->Draw();
    textSprite->SetTextColor(240,240,240,0);//back to white, (it may become gray)
    textSprite->LoadFromText(text);
}
