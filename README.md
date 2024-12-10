# pear

pear is a work-in-progress 3d engine, designed with portability in mind. Its current graphics API is OpenGL, and only support Linux (for now).

## Compiling from source

First, install the following dependencies :
- GLFW
- GLEW
- cglm
- assimp

Then execute the following command to generate the project (makefiles on Linux, VS solution on Windows):

```bash
./generate-projects.sh
```

On Windows, simply open the `.sln` solution file. On Linux, open the directory in your favorite text editor, you can compile the code with `make`.

## pear-format

pear uses a custom format to define 3D models and textures. The engine can't load any other file formats by itself. To load any 3D model, you first need to convert it to the right format using the converter.

## Projects

This repository contains multiple projects:
- `pear` is the engine itself.
- `pear-project` the project that uses the pear engine.
- `pear-formats` is a crude library to import and export 3D files and textures.
- `converter` is a program to convert from most 3D model formats to the pear format.
