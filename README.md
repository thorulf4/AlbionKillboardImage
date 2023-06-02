# Albion Killboard Image
A C++ program to convert a Albion killboard link into a image summary using OpenCV. Plus a discord integration


## Image generator

A image generation library that takes a killboard link like https://albiononline.com/killboard/kill/792676849 and turns it into an image summary.
Features:
- Item image caching
- Gear price evaluation

![image_example](https://github.com/thorulf4/AlbionKillboardImage/assets/10059450/0ddb747f-fa4c-40dc-859a-bbc22024549c)

## Discord integration
Adds a slash command that allows users to create image summaries.

Discord token must be supplied in the source code in `src/bot/bot.cpp`
![image](https://github.com/thorulf4/AlbionKillboardImage/assets/10059450/37393ad8-59f4-4e9c-a9f2-2ac5488597f0)

## Building
Build using cmake, all dependencies should be fetched automatically.
Note images in `resources` should be moved next to built executable (Will be handled with cmake install later)
