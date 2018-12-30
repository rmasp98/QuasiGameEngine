# Quasi Game Engine

This is going to eventually be a game engine... maybe. The dream is to have it as a cross platform, cross API game engine but at this point I am just focusing on Linux (Arch) using OpenGL.

The engine is currently seperated into 3 parts:
 * Resource - responsible for loading, maintaining and deleting all resources such as meshs, textures, etc.
 * Renderer - responsible for interaction with the graphics API, so it sends all resource data to graphics card and sends rendering instructions
 * Interface - responsible for interacton with OS (maybe...). It currently handles the window and user input
 * Utils - this is just a set of tools for the other parts. It currently has file management (including json), memory management and a logging system. This will likely grow as the engine does

The engine will also have a few more parts in the future:
 * Physics - responsible for things like collisions, forces, etc. Probably will just use openDE to start off with but want to try and build my own (for fun!)
 * UI - this will eventually be a proper game engine and will have a UI to build out your game. Don't expect anything special :P
 * Maybe more but have not got that far yet...

Finally the development board: https://tree.taiga.io/project/rmasp98-game-engine/ 
