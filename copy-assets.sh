#!/bin/sh

if [ -d "bin/Debug-linux-x86_64/pear-project" ]; then
    cp -R pear/assets/* bin/Debug-linux-x86_64/pear-project
    cp -R pear-project/assets/* bin/Debug-linux-x86_64/pear-project
else
    echo "binary directories do not exist yet! build pear before running this script"
fi
