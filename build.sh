#!/bin/bash
cwd="$(pwd)"
mkdir /tmp/build && cd /tmp/build
wget https://img2text.aptjet.dpdns.org/img2txt.c
wget https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h
gcc -Os img2txt.c -o /usr/local/bin/img2txt -lm
cd "$cwd"
rm -rf /tmp/build
