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
        if (playlistStream)
        {
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
        }
        else
        {
            Console::error("Can not load %s.", path.c_str());
        }
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
            _playIndex = -1;
            _playIndex = getNextPlayIndex();
        }
        setVolume(getVolume());
        play();
    }

    virtual void play() override
    {
        if (!isPlaying())
        {
            if (_mediaPlayer->isClosed())
            {
                openCurrentMedia();
            }
            _playing = true;
        }
    }

    virtual int getNextPlayIndex()
    {
        int playIndex = _playIndex + 1;
        if (playIndex > _playlist.size() - 1)
        {
            // Forward to the first item
            playIndex = 0;
        }
        return playIndex;
    }

    virtual int getPrevPlayIndex()
    {
        int playIndex = _playIndex - 1;
        if (_playIndex < 0)
        {
            // Back to the last item
            _playIndex = _playlist.size() - 1;
        }
        return playIndex;
    }

    virtual void next() override
    {
            _playIndex = getNextPlayIndex();
        openCurrentMedia();
        play();
    }

    virtual void prev() override
    {
        _playIndex = getPrevPlayIndex();
        openCurrentMedia();
        play();
    }

    virtual void openCurrentMedia()
    {
        if (_playlist.size() == 0)
        {
            _playIndex = 0;
            return;
        }
        if (_playIndex < 0)
        {
            _playIndex = _playlist.size() - 1;
        }
        else if (_playIndex > _playlist.size() - 1)
        {
            _playIndex = 0;
        }
        String location = _playlist[_playIndex];
        Console::info("Playing %s", location.c_str());
        _mediaPlayer->open(location);
    }

protected:
    vector<String> _playlist;
    int _playIndex = 0;
};

#endif
