#ifndef BASE_LIST_POD_H
#define BASE_LIST_POD_H

#include <Arduino.h>

#include <vector>

#include <Console.h>
#include <FS.h>

#include "BasePod.h"

using namespace std;

class BaseListPod : public BasePod
{
public:
    BaseListPod(MediaPlayer* mediaPlayer) : BasePod(mediaPlayer)
    {

    }



    virtual void loadPlaylist()
    {

    }

    virtual void loadPlayListFromLocal(String path)
    {
        clearPlaylist();
        Console::info("Loading playlist from %s...", path.c_str());
        File playlistStream = SPIFFS.open(path, "r");
        while (playlistStream.available())
        {
            String line = playlistStream.readStringUntil('\n');
            line.trim();
            if (line.length() == 0 || line.startsWith("#"))
            {
                continue;
            }
            addToPlaylist(line);
        }
        playlistStream.close();
        Console::info("%d items has been loaded to the playlist.", _playlist.size());
        _playIndex = 0;
    }

    virtual void addToPlaylist(String location)
    {
        _playlist.push_back(location);
    }

    virtual void clearPlaylist()
    {
        _playlist.clear();
    }




    virtual void activate()
    {
        if (_playlist.size() == 0)
        {
            loadPlaylist();
            _playIndex = 0;
        }
        setVolume(getVolume());
        openAndPlayCurrentMedia();
    }

    virtual void next() override
    {
        _playIndex++;
        if (_playIndex > _playlist.size() - 1)
        {
            // Cycling
            _playIndex = 0;
        }
        openAndPlayCurrentMedia();
    }

    virtual void prev() override
    {
        _playIndex--;
        if (_playIndex < 0)
        {
            // Cycling
            _playIndex = _playlist.size() - 1;
        }
        openAndPlayCurrentMedia();
    }

    virtual void openAndPlayCurrentMedia()
    {
        String location = _playlist[_playIndex];
        Console::info("Playing %s", location.c_str());
        _mediaPlayer->open(location);
        play();
    }

protected:
    vector<String> _playlist;
    int _playIndex = 0;
};

#endif
