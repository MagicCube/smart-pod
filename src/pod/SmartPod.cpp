#include "SmartPod.h"

#include <Console.h>

SmartPod::SmartPod() : _vs1053(VS1053_XCS_PIN, VS1053_XDCS_PIN, VS1053_DREQ_PIN), _mediaPlayer(&_vs1053)
{

}

bool SmartPod::begin()
{
    bool vs1053Enabled = _vs1053.begin();
    if (!vs1053Enabled)
    {
        return false;
    }
    // Set the initial volume
    setVolume(80);
    return true;
}

void SmartPod::switchMode(SmartPodMode mode)
{
    _mode = mode;
    if (mode == RADIO)
    {
        Console::info("Switch to [Radio] mode.");
        _mediaPlayer.open("http://http.qingting.fm/387.mp3"); // NCR Finance
        //mediaPlayer.open("http://http.qingting.fm/4963.mp3"); // Nanjing Music Radio
    }
    else if (mode == FAV_MUSIC_LIST)
    {
        Console::info("Switch to [FavMusicList] mode.");
        //mediaPlayer.open("/test.mp3");
        _mediaPlayer.open("http://m2.music.126.net/NG4I9FVAm9jCQCvszfLB8Q==/1377688074172063.mp3");
    }
}

void SmartPod::switchMode()
{
    if (_mode == RADIO)
    {
        switchMode(FAV_MUSIC_LIST);
    }
    else if (_mode == FAV_MUSIC_LIST)
    {
        switchMode(RADIO);
    }
}

void SmartPod::handle()
{
    _mediaPlayer.handle();
}




uint8_t SmartPod::getVolume()
{
    return _vs1053.getVolume();
}

void SmartPod::setVolume(int volume)
{
    if (volume > 100)
    {
        volume = 100;
    }
    else if (volume < 0)
    {
        volume = 0;
    }
    _vs1053.setVolume(volume);
    Console::info("Setting volume to %d.", volume);
}

void SmartPod::setVolumeUp()
{
    setVolume(getVolume() + 5);
}

void SmartPod::setVolumeDown()
{
    setVolume(getVolume() - 5);
}
