#include "textfield.h"

const char BACKSPACE = 8;

TextField::TextField(int x, int y, int w, int h, SDL_Setup* passedSdlSetup, std::string passedTitle, std::string defaultText)
{
    sdlSetup = passedSdlSetup;

    staticCamera = 0;

    sizeCoefficientW = (w/166.0);
    sizeCoefficientH = (h/40.0);

    inputText = defaultText;
    inputTextSprite = new Sprite(sdlSetup->GetRenderer(),"error.png",x+6*sizeCoefficientW+3,y+16*sizeCoefficientH+1,60*sizeCoefficientW,100*sizeCoefficientH,&staticCamera,&staticCamera);
    inputTextSprite->SetFontSize(14*sizeCoefficientH);
    inputTextSprite->LoadFromText(inputText);
    //inputTextSprite->SetCropping(0,0,200,20);

    textFieldSprite = new Sprite(sdlSetup->GetRenderer(),"textField.png",x,y,w,h,&staticCamera,&staticCamera);
    textFieldUnselectableSprite = new Sprite(sdlSetup->GetRenderer(),"textFieldUnselectable.png",x,y,w,h,&staticCamera,&staticCamera);

    title = passedTitle;
    textFieldTitleSprite = new Sprite(sdlSetup->GetRenderer(),"error.png",x+sizeCoefficientW+5,y+sizeCoefficientH-2,100*sizeCoefficientW,30*sizeCoefficientH,&staticCamera,&staticCamera);
    textFieldTitleSprite->SetFontSize(14*sizeCoefficientH);
    textFieldTitleSprite->LoadFromText(title);

    selected = false;
    selectable = true;

    lastTextInputTime = SDL_GetTicks();
    lastInputKey = '\0';

    showCursor = false;
    cursorShowTimeCheck = SDL_GetTicks();

    hidden = false;
}

TextField::~TextField()
{
    delete inputTextSprite;
    delete textFieldSprite;
    delete textFieldTitleSprite;
}

void TextField::Draw()
{
    if(!hidden)
    {
        if(selectable)
        {
            textFieldSprite->Draw();
        }
        else
        {
            textFieldUnselectableSprite->Draw();
        }

        inputTextSprite->Draw();
        textFieldTitleSprite->Draw();
    }
}

void TextField::Update()
{
    GetMouseInput();

    if(GetSelected())
    {
        int inputDelay = 150;//0.2 second
        if(sdlSetup->GetMainEvent()->type == SDL_KEYDOWN)
        {
            if(sdlSetup->GetMainEvent()->key.keysym.sym == SDLK_BACKSPACE && inputText.size()>0)
            {
                if(lastInputKey == BACKSPACE)
                {
                    if(SDL_GetTicks() - lastTextInputTime > (unsigned int)inputDelay)
                    {
                        lastTextInputTime = SDL_GetTicks();
                        lastInputKey = BACKSPACE;
                        inputText.erase(inputText.size()-1);
                    }
                }
                else
                {
                    lastTextInputTime = SDL_GetTicks();
                    lastInputKey = BACKSPACE;
                    inputText.erase(inputText.size()-1);
                }
            }

        }
        if(sdlSetup->GetMainEvent()->type == SDL_TEXTINPUT)
        {
            if(inputTextSprite->GetWidth()<140*sizeCoefficientW)
            {
                if(' ' <= *sdlSetup->GetMainEvent()->text.text && *sdlSetup->GetMainEvent()->text.text <= 'z')
                {
                    if(lastInputKey == *sdlSetup->GetMainEvent()->text.text)
                    {
                        if(SDL_GetTicks() - lastTextInputTime > (unsigned int)inputDelay)
                        {
                            lastTextInputTime = SDL_GetTicks();
                            lastInputKey = *sdlSetup->GetMainEvent()->text.text;
                            inputText += sdlSetup->GetMainEvent()->text.text;
                        }
                    }
                    else
                    {
                        lastTextInputTime = SDL_GetTicks();
                        lastInputKey = *sdlSetup->GetMainEvent()->text.text;
                        inputText += sdlSetup->GetMainEvent()->text.text;
                    }
                }
            }
        }


        std::string tempInputText = inputText;

        if(showCursor)
        {
            tempInputText += '|';
        }
        if(SDL_GetTicks() - cursorShowTimeCheck > 500)
        {
            cursorShowTimeCheck = SDL_GetTicks();
            showCursor = !showCursor;
        }

        inputTextSprite->LoadFromText(tempInputText.c_str());
    }
}

void TextField::GetMouseInput()
{
    if(selectable)
    {
        if(sdlSetup->MouseButtonLeftWasPressed())
        {
            int mouseX = sdlSetup->GetMainEvent()->button.x;
            int mouseY = sdlSetup->GetMainEvent()->button.y;

            if((textFieldSprite->GetX() <= mouseX && mouseX <= textFieldSprite->GetX() + textFieldSprite->GetWidth())
                                             &&
           (textFieldSprite->GetY() <= mouseY && mouseY <= textFieldSprite->GetY() + textFieldSprite->GetHeight()))
            {
                selected = true;// Select
            }
            else
            {
                Unselect();//Unselect
            }
        }

        if(sdlSetup->KeyWasPressed(SDL_SCANCODE_RETURN))
        {
            Unselect();
        }
    }
}

void TextField::SetText(std::string text)
{
    inputText = text;
    std::string tempInputText = inputText + " ";//to prevent text with zero width
    inputTextSprite->LoadFromText(tempInputText.c_str());
}

void TextField::SetSelectable(bool mode)
{
    selectable = mode;
    if(selectable)
    {
        inputTextSprite->SetTextColor(240,240,240,0);//back to white
        inputTextSprite->LoadFromText(inputText);

        if(hidden)
        {
            hidden = false;
        }
    }
    else
    {
        inputTextSprite->SetTextColor(150,150,150,0);//lite gray
        inputTextSprite->LoadFromText(inputText);
    }
}

void TextField::Unselect()
{
    selected = false;

    if(showCursor)
    {
        showCursor = false;
        inputTextSprite->LoadFromText(inputText);
    }
}
