import request from "request-promise";
import fs from "fs";

const NETEASE_USER_ID = "34176019";
const apiUrl = `http://music.163.com/api/playlist/detail?id=${NETEASE_USER_ID}`;

async function loadPlaylist()
{
    const res = await request({
        uri: apiUrl,
        json: true
    });

    let m3u = "#EXTM3U\n";
    res.result.tracks.forEach(track => {
        m3u += track.mp3Url + "\n";
    });

    fs.writeFile("../data/fav-music-pod.m3u", m3u);
}

loadPlaylist();
