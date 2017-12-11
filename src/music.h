#ifndef MUSIC_H
#define MUSIC_H
#include "rpg.h"

class Music
{
    public:
        Music(std::string fileName);
        ~Music();
        void Play(int volume);
        static void Pause(){Mix_PauseMusic();}
        static bool Paused(){return Mix_PausedMusic()==1;}
        static void Resume(){Mix_ResumeMusic();}
        static void Stop(){Mix_HaltMusic();}
    private:
        Mix_Music* music;
};

#endif // MUSIC_H
