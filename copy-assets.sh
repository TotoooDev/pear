#!/bin/sh

if [ -d "bin/Debug-linux-x86_64/editor" ]; then
    cp -R pear/assets/* bin/Debug-linux-x86_64/editor
    cp -R editor/assets/* bin/Debug-linux-x86_64/editor
else
    echo "binary directories do not exist yet! build pear before running this script"
fi
