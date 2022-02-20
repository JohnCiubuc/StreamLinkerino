## Official Chatterino Implementation

Integration of this application into a first-class chatterino feature is currently in process. Pull request can be [viewed here](https://github.com/Chatterino/chatterino2/pull/2854)

**Testing branch does not require chatterino to be patched**, but it does have a considerable delay (5-10s) on switching streams due to chatterino json writing frequency.

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

    
## Features

1. High performance, low memory, and responsive

2. No mid-roll ads (Thanks streamlink!)

3. MPV player features:

    * Pause
    * Rewind
    * Fast forward
    * And more!

4. Streamlink and Chatterino Integration

5. Seamless stream switching (Chatterino patch required)

## Building 

Building streamlinkerino

Contributed by [dewcked](https://github.com/dewcked) from [Release #22](https://github.com/JohnCiubuc/StreamLinkerino/issues/22):


1. Install docker and git
2. `git clone https://github.com/JohnCiubuc/streamlinkerino.git --recurse-submodules`
3. `git submodule update --recursive --remote`
4. `cd streamlinkerino`
5. Pull docker image from [Here](https://github.com/OlivierLDff/QtLinuxCMakeDocker).
 * Linux) `docker run -it --rm -v $(pwd):/src/ --device /dev/fuse --cap-add SYS_ADMIN --security-opt apparmor:unconfined reivilo1234/qt-linux-cmake:qt5.15.1 bash`
 * Windows) `docker run -it --rm -v %CD%:/src/ --device /dev/fuse --cap-add SYS_ADMIN --security-opt apparmor:unconfined reivilo1234/qt-linux-cmake:qt5.15.1 bash`
6. `mkdir build && cd build`
7. `cmake ../src && make`


## Patching Chatterino

1.  Copy `chatterino.patch` from the streamlinkerino Patch directory, into chatterino's submodule project directory (same location as `chatterino.pro`)
2. `patch -p0 < chatterino.patch`
3. Create build folder `mkdir build && cd build`
4. Go into `build` directory
5.   `qmake .. && make`


## TODO:

~~1. Detect if streamlink is installed, and if not, prompt user~~

~~2. Detect if chatterino is installed, and if not, prompt user~~

~~3. On switching streams, setup two mpv clients -- one playing current stream and the other loading new stream. When new stream finishes loading, swap to the new client~~

4. Currently can only change stream based on chatterino. Add option to change stream ignoring chatterino (or if chatterino isn't patched)

5. Resize chatterino window

~~6. Create a settings dialog to specify streamlink/chatterino location and settings~~

~~7. Link the settings button in chatterino (if patched) with the settings dialogue in streamlinkerino~~

8. Auto apply patch and build chatterino together with streamlinkerino via CMakeLists


	
## Screenshots
![ss1](https://github.com/JohnCiubuc/JohnCiubucGifs/raw/main/screenshots/streamlinkerino1.png)

(Older Version)
<p align="center">
  <img src="https://raw.githubusercontent.com/JohnCiubuc/JohnCiubucGifs/main/streamlinkerino.gif" />
</p>
(Current Version)
<p align="center">
  <img src="https://raw.githubusercontent.com/JohnCiubuc/JohnCiubucGifs/main/streamlinkerino2.gif" />
</p>
