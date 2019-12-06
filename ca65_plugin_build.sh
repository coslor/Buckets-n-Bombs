#!/bin/bash
cd /mnt/c/Users/chris/Projects/cc65/Buckets n Bombs
C:\Users\chris\cc65\bin/cc65 -verbose -g  -t c64  src/Buckets n Bombs.c
C:\Users\chris\cc65\bin/ca65 -verbose -g  src/Buckets n Bombs.s
C:\Users\chris\cc65\bin/ca65 -verbose -g  src/sprite_data.s
C:\Users\chris\cc65\bin/ld65 -t c64 -Ln Buckets.c64.lbl -o Buckets.c64 **/*.o C:\Users\chris\cc65\bin/../lib/c64.lib