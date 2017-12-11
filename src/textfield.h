#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "sprite.h"
#include "sdl_setup.h"

class TextField
{
public:
    TextField(int x, int y, int w, int h, SDL_Setup* passedSdlSetup, std::string passedTitle, std::string defaultText = "");
    void Draw();
    void Update();

    //if the text field is selected
    bool GetSelected(){return selected;}
    std::string GetText(){return inputText;}
    void SetText(std::string text);
    void SetSelectable(bool mode);
    void Hide(){SetSelectable(false); hidden = true;}
    void Unhide(){SetSelectable(true);}
    ~TextField();
private:
    void Unselect();

    void GetMouseInput();
    Sprite* inputTextSprite;
    std::string inputText;

    Sprite* textFieldSprite;
    Sprite* textFieldUnselectableSprite;
    Sprite* textFieldTitleSprite;
    std::string title;

    bool selected;
    bool selectable;
    int lastTextInputTime;
    char lastInputKey;
    bool showCursor;
    int cursorShowTimeCheck;

    bool hidden;

    float sizeCoefficientW;
    float sizeCoefficientH;

    int staticCamera;

    SDL_Setup* sdlSetup;
};

#endif // TEXTFIELD_H
