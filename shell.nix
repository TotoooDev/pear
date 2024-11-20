{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
    nativeBuildInputs = with pkgs; [
        premake5
        glfw
        glew
        cglm
        assimp
        gdb
    ];
}