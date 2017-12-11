#include "music.h"
#include "soundeffect.h"
Music::Music(std::string fileName)
{
    fileName = SoundEffect::SOUND_FOLDER_PATH + fileName;
    music = NULL;
    music = Mix_LoadMUS(fileName.c_str());
    if(music == NULL)
    {
        DebugCOut("Can't load " + fileName);
    }
}

Music::~Music()
{
    Mix_FreeMusic(music);
}

void Music::Play(int volume)
{
    if(Mix_PlayingMusic() == 0)
    {
        Mix_VolumeMusic(volume);
        Mix_PlayMusic(music,0);
    }
}
