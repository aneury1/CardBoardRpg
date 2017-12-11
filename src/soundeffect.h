#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H
#include "rpg.h"

class SoundEffect
{
    public:
        SoundEffect(std::string fileName, int volume);
        ~SoundEffect();
        void Play();
    private:
        Mix_Chunk* sample;

        static const std::string SOUND_FOLDER_PATH;
        friend class Music;
};

#endif // SOUNDEFFECT_H
