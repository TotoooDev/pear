# pear

pear is a work-in-progress 3d game engine, focused on portability.

## compiling

pear uses [premake](https://premake.github.io/) as its build system, so you will need to install on your system. place the executable somewhere in your path, and you'll be good to go!

the first step is to clone this repo. you can do this by running `git clone --recursive https://github.com/TotoooDev/pear`.

### linux

first you will need to install a few depedencies. they are needed for glfw to compile properly. you can find which packages to install on the [glfw compilation guide](https://www.glfw.org/docs/latest/compile.html#compile_deps).

once you've installed the depedencies, run `premake5 gmake` inside of the repository. this will generate the makefiles required to build the engine.

finally, you can run `make`, and the engine will compile! the binary will be located inside the bin/[config]-linux/editor directory.

### windows

coming soon!

## assets

the assets used by pear are located in `pear/assets` and `editor/assets`. they respectively contain assets needed for the engine to work properly (shaders and stuff), and the assets needed for the editor to work (images, scripts, ...). all the assets are copied into the working directory after compilation. you can also copy the assets manually by executing the `copy-assets.sh` script.

apart from the assets, shared libraries (.so and .dll files) are also copied in the working directory after compilation.
