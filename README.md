
## streamlinkerino
Ever wanted to use StreamLink + MPV + Chatterino all in one application? Look no further, as this project has your solution!

streamlinkerino embeds MPV (using [StreamLink](https://streamlink.github.io/index.html) as its base) and [Chatterino](https://chatterino.com/) into one application. If you patch Chatterino [(Git Link)](https://github.com/Chatterino/chatterino2) with the supplied patch file, the MPV stream will auto-update if you switch channels in Chatterino!

## Why does this application exist

This project was created for the following reasons:
1. Avoiding usage of the twitch web application
	* It is unfortunately laggy and resource heavy
	* Embedded stream ads can not be avoided
	* Twitch Player is feature poor (compared to other players)
	* Several bug with streams not loading when using third party extensions like FFZ/BTTV
2. Avoiding Electron / JS applications
	* Alternatives such as [StreamLink Twitch GUI](https://streamlink.github.io/streamlink-twitch-gui/) do exist, but you still have to deal with the resource heaviness of electron-based apps and the website itself
3. Performance
	* The base app and chatterino is C++/Qt. The Player is MPV and StreamLink is used for an ad-free stream experience. 
	* The result is a light weight, high performance twitch client

## Building 

Building streamlinkerino

Edit `line 17` in `mainwindow.cpp` to the correct location of chatterino
```bash
git clone https://github.com/JohnCiubuc/streamlinkerino.git
cd streamlinkerino
mkdir build && cd build
cmake .. && make
```
## Patching Chatterino

1. Download Chatterino's [Source Code](https://github.com/Chatterino/chatterino2#Building)
2. Install necessary [dependencies](https://github.com/Chatterino/chatterino2/blob/master/BUILDING_ON_LINUX.md), but do **not** build yet
3.  Copy `chatterino.patch` from the streamlinkerino Patch directory, into chatterino's project directory (same location as `chatterino.pro`)
4. `patch -p0 < chatterino.patch`
5. Create build folder `mkdir build && cd build`
6. Go into `build` directory
7.   `qmake .. && make`


## TODO:
1. Detect if streamlink is installed, and if not, prompt user
2. Detect if chatterino is installed, and if not, prompt user
3. On switching streams, setup two mpv clients -- one playing current stream and the other loading new stream. When new stream finishes loading, swap to the new client
4. Currently can only change stream based on chatterino. Add option to change stream ignoring chatterino (or if chatterino isn't patched)
5. Resize chatterino window


	
## Screenshots
![ss1](https://github.com/JohnCiubuc/StreamLinkerino/raw/master/screenshots/ss1.png)

<p align="center">
  <img src="https://raw.githubusercontent.com/JohnCiubuc/JohnCiubucGifs/main/streamlinkerino.gif" />
</p>
