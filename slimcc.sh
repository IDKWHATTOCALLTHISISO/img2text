#!/bin/bash
# install: wget -qO- https://img2text.aptjet.dpdns.org/slimcc.sh | bash
set -e
cwd="$(pwd)"
mkdir /tmp/build && cd /tmp/build
wget https://img2text.aptjet.dpdns.org/img2text.c
wget https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h
slimcc -Os img2text.c -o /usr/local/bin/img2text -lm
cd "$cwd"
rm -rf /tmp/build
