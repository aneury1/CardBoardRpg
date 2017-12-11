#ifndef SDL2_H
#define SDL2_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cmath>

void DebugCOut(std::string text);

#include <sstream>
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

bool CheckLevelForExistance(std::string levelName);

double DegreesToRadians(double degree);

enum GameMode
{
    GM_GAMEPLAY,
    GM_LEVELEDIT
};

/*inherit this, if you don't want your class to be copied
class Noncopy
{
protected:
    Noncopy(){}
    ~Noncopy(){}
private:
    Noncopy(const Noncopy& other);
    const Noncopy& operator=(const Noncopy& other);
};*/

#endif // SDL2_H
