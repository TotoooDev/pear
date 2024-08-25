# pear

a cool 3d engine

## projects

- `pear` is the engine itself
- `pear-project` is the game that uses pear as the engine
- `pear-3d` is the (crude) library to load and save the custom 3d file format. it saves models in the pear3d format and images (like textures) in the pear3d-image format
- `pear-3d-converter` is a small program that converts any 3d file into the pear3d format

pear itself can only load pear3d models. you need to convert your models first before you can use them with pear.

# dependencies

## linux

### pear

- GLFW
- GLEW
- cglm

### pear-3d

- cglm

### pear-3d-converter

- cglm
- assimp
