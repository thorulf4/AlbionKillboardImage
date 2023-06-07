# Albion Killboard Image
A C++ program to convert a Albion killboard link into a image summary using OpenCV. Plus a discord integration.

This repository has two components a Image generationg library and a discord integration.


## Image generator

A image generation library that takes a killboard link like https://albiononline.com/killboard/kill/792676849 and turns it into an image summary.

Features:
- Item image caching
- Gear price evaluation

![image_example](https://github.com/thorulf4/AlbionKillboardImage/assets/10059450/0ddb747f-fa4c-40dc-859a-bbc22024549c)

## Discord integration
Adds a slash command that allows users to create image summaries.

Discord token can be provided in two ways either directly in the terminal or stored in a separate file.

`./bot.sh --token my-secret`\
`./bot.sh --token-file secret-file`

![image](https://github.com/thorulf4/AlbionKillboardImage/assets/10059450/37393ad8-59f4-4e9c-a9f2-2ac5488597f0)

# Building
This was tested using
 - cmake 3.26.3
 - g++ 12.1.0

The library and discord integration can be built using cmake. 
The code can be statically linked with the cmake option `-DBUILD_SHARED_LIBS=OFF`
```
cmake -B build -DBUILD_DISCORD_BOT=ON
cmake --build build --target bot
```
Optionally you can install it although the executable name `bot`
```
cmake --install build --prefix ~/path-to-bot
```