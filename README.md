# SiegeCraft

SiegeCraft is an early-stage 2D game and graphics prototype developed in C++ with OpenGL. The project was built to explore low-level graphics programming concepts without relying on a complete game engine.

It demonstrates textured rendering, sprite-sheet animation, camera movement, real-time input handling, shader programming, and basic GPU resource management.

## Features

* OpenGL 4.1 Core Profile rendering
* SDL2 window creation, input handling, and timing
* GLAD-based OpenGL function loading
* GLM matrix and camera transformations
* GLSL vertex and fragment shaders
* Texture loading with `stb_image`
* Animated character rendering from a sprite sheet
* Frame-rate-independent animation using delta time
* Textured ground rendering
* Keyboard-controlled camera movement
* Mouse-wheel and keyboard zoom controls
* OpenGL error-checking utilities

## Controls

| Input                | Action                   |
| -------------------- | ------------------------ |
| `W` or `Up Arrow`    | Move the camera upward   |
| `S` or `Down Arrow`  | Move the camera downward |
| `A` or `Left Arrow`  | Move the camera left     |
| `D` or `Right Arrow` | Move the camera right    |
| Mouse wheel          | Zoom in or out           |
| Numpad `+`           | Zoom in                  |
| Numpad `-`           | Zoom out                 |
| Close window         | Exit the application     |

## Technologies

* C++
* OpenGL
* GLSL
* SDL2
* GLAD
* GLM
* stb_image
* Microsoft Visual Studio

## Project Structure

```text
SiegeCraft/
├── main.cpp                   # Application setup, rendering, input, and main loop
├── animation.cpp              # Sprite-animation implementation
├── animation.hpp              # Animation classes and declarations
├── camera.cpp                 # Camera movement and zoom implementation
├── camera.hpp                 # Camera class declaration
├── glad.c                     # OpenGL function loader implementation
├── shaders/
│   ├── vss.glsl               # Vertex shader
│   └── fss.glsl               # Fragment shader
├── sprites/
│   └── anim1/                 # Character animation frames and sprite sheet
├── textures/                  # Ground, grass, and floor textures
└── SiegeCraft.vcxproj.xml     # Original Visual Studio project configuration
```

## How It Works

The application creates an SDL2 window and an OpenGL 4.1 Core Profile context. Mesh data is transferred to the GPU through vertex buffers, index buffers, and vertex array objects.

The character is rendered as a textured quad. Its animation is produced by selecting different regions of a sprite sheet and updating the texture coordinates according to the current animation frame. Delta time is used to keep the animation timing independent of the rendering frame rate.

The camera uses GLM view transformations and can be moved or zoomed through SDL2 keyboard and mouse input.

## Building the Project

This project was originally developed on Windows with Microsoft Visual Studio.

Required libraries:

* SDL2
* GLM
* GLAD
* stb_image

The original Visual Studio project contains machine-specific include and library paths. To build it on another computer, update the C/C++ include directories and linker library directories so that they point to your installed dependency locations.

The project links against:

```text
SDL2.lib
SDL2main.lib
```

It also requires `SDL2.dll` to be available beside the built executable or somewhere in the system path.

## Project Status

This repository contains a functional graphics and gameplay prototype rather than a finished commercial game. Its primary purpose was to develop practical experience with C++, OpenGL rendering, shaders, animation systems, texture handling, and real-time application structure.


## Author

**Kerem Günay**

* GitHub: [keremgunay](https://github.com/keremgunay)
* Repository: [SiegeCraft](https://github.com/keremgunay/SiegeCraft)
