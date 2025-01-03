# pear

pear is a work-in-progress 3D engine, designed with portability in mind. Its current graphics API is OpenGL, and only support Linux (Windows support is planned for the near future, but i primarly use Linux, so Windows might get updates later).

## Compiling from source

First, install the following dependencies :
- GLFW
- GLEW
- cglm

Then execute the following script to generate the project (makefiles on Linux):

```bash
./generate-projects.sh
```

Then, run this script to copy the assets to the binaries directory:

```bash
./copy-assets.sh
```

Now, you can run `make` to compile the project.

Once this is finished, your game is ready to run in the `bin/Debug-linux-x86_64/pear-project` directory!

## Projects

This repository contains multiple projects:
- `pear` is the engine itself.
- `pear-project` the project that uses the pear engine.
