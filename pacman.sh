#!/bin/bash
export XDG_CONFIG_HOME=$PWD/conf/
xfce4-terminal --disable-server --command="./src/pacman-level-editor $*"

