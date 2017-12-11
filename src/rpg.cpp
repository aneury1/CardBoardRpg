#include "rpg.h"
#include <fstream>
#include <cmath>

void DebugCOut(std::string text)
{
    #ifdef DEBUG
        std::cerr << text << std::endl;
    #endif // DEBUG
}

bool CheckLevelForExistance(std::string levelName)
{
    if(levelName == "")
        return false;
    std::string levelPath = "data/maps/" + levelName + ".txt";
    return std::ifstream(levelPath.c_str()).good();
}

double DegreesToRadians(double degree)
{
    return (degree * (M_PI/180.0));
}
