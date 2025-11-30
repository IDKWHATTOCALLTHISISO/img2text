#!/bin/bash
# install: wget -qO- https://img2text.aptjet.dpdns.org/build.sh | bash
set -e
cwd="$(pwd)"
mkdir /tmp/build && cd /tmp/build
wget https://img2text.aptjet.dpdns.org/img2txt.c
wget https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h
gcc -Os --std=gnu99 img2txt.c -o /usr/local/bin/img2txt -lm
cd "$cwd"
rm -rf /tmp/build
