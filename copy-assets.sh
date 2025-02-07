#!/bin/sh

# first argument is the location of the root of the git repo

git_root=""

if [ $# -gt 0 ]; then
    git_root=$1
fi

if [ -d "./$git_root/bin/debug-linux/editor" ]; then
    # assets
    cp -R ./$git_root/pear/assets/* ./$git_root/bin/debug-linux/editor
    cp -R ./$git_root/editor/assets/* ./$git_root/bin/debug-linux/editor

    # shared libs
    cp ./$git_root/pear/vendor/imgui/bin/opengl/debug/libimgui.so ./$git_root/bin/debug-linux/editor/libimgui.so
    cp ./$git_root/pear/vendor/lua/bin/opengl/debug/liblua.so ./$git_root/bin/debug-linux/editor/liblua.so
else
    echo "binary directories do not exist yet! build pear before running this script"
fi
