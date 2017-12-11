#include "soundeffect.h"

const std::string SoundEffect::SOUND_FOLDER_PATH = "data/sounds/";

SoundEffect::SoundEffect(std::string fileName, int volume)
{
    sample = NULL;

    fileName = SOUND_FOLDER_PATH + fileName;
    sample = Mix_LoadWAV(fileName.c_str());
    if(sample == NULL)
    {
        DebugCOut("Can't load " + fileName);
    }
    Mix_VolumeChunk(sample,volume);
}

SoundEffect::~SoundEffect()
{
    Mix_FreeChunk(sample);
}

void SoundEffect::Play()
{
    Mix_PlayChannel(-1,sample,0);
}
