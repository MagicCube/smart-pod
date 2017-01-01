#include "SmartPod.h"

#include <Console.h>

SmartPod::SmartPod() : _vs1053(VS1053_XCS_PIN, VS1053_XDCS_PIN, VS1053_DREQ_PIN),
                       Pod(NULL)
{
    _mediaPlayer = new MediaPlayer(&_vs1053);
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
    // Deactive the current Pod
    if (_activePod)
    {
        _activePod->deactivate();
    }

    _mode = mode;
    if (_mode == RADIO_POD)
    {
        Console::info("Switch to [RadioPod] mode.");
        if (!_radioPod)
        {
            _radioPod = new RadioPod(_mediaPlayer);
        }
        _activePod = _radioPod;
    }
    else if (_mode == FAV_MUSIC_POD)
    {
        Console::info("Switch to [FavMusicPod] mode.");
        if (!_favMusicPod)
        {
            _favMusicPod = new FavMusicPod(_mediaPlayer);
        }
        _activePod = _favMusicPod;
    }

    activate();
}

void SmartPod::switchMode()
{
    if (_mode == RADIO_POD)
    {
        switchMode(FAV_MUSIC_POD);
    }
    else if (_mode == FAV_MUSIC_POD)
    {
        switchMode(RADIO_POD);
    }
}



void SmartPod::activate()
{
    if (_activePod)
    {
        _activePod->activate();
    }
}

void SmartPod::deactivate()
{
    if (_activePod)
    {
        _activePod->deactivate();
    }
}

void SmartPod::handle()
{
    if (_activePod)
    {
        _activePod->handle();
    }
}

bool SmartPod::isPlaying()
{
    if (_activePod)
    {
        return _activePod->isPlaying();
    }
    else
    {
        return false;
    }
}

void SmartPod::play()
{
    if (_activePod)
    {
        _activePod->play();
    }
}

void SmartPod::pause()
{
    if (_activePod)
    {
        _activePod->pause();
    }
}

void SmartPod::playPause()
{
    if (_activePod)
    {
        if (isPlaying())
        {
            pause();
        }
        else
        {
            play();
        }
    }
}

void SmartPod::stop()
{
    if (_activePod)
    {
        _activePod->stop();
    }
}

void SmartPod::next()
{
    if (_activePod)
    {
        _activePod->next();
    }
}

void SmartPod::prev()
{
    if (_activePod)
    {
        _activePod->prev();
    }
}

uint8_t SmartPod::getVolume()
{
    if (_activePod)
    {
        return _activePod->getVolume();
    }
    else
    {
        return 0;
    }
}

void SmartPod::setVolume(uint8_t volume)
{
    if (_activePod)
    {
        _activePod->setVolume(volume);
    }
}

void SmartPod::setVolumeUp()
{
    setVolume(getVolume() + 5);
}

void SmartPod::setVolumeDown()
{
    setVolume(getVolume() - 5);
}
