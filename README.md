# CYD fan platformer

Old fan build for the ESP32-2432S028 (CYD). Not an official Nintendo game.

Levels load from MARIO.txt on a FAT32 SD card. Controller is ESP-NOW.

## Files

- CYD_Mario_v4_3.ino — game sketch
- MARIO.txt — levels

## Build

Board: ESP32 Dev Module. Library: TFT_eSPI. Partition: Huge APP if the sketch does not fit.
