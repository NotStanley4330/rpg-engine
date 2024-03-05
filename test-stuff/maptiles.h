//
// Created by Michael Norris on 3/4/2024.
//
#include <windows.h>
#include <winuser.h>

#ifndef RPG_ENGINE_MAPTILES_H
#define RPG_ENGINE_MAPTILES_H

#define DEFAULT_TILE_SIZE 64
#define DEFAULT_TILES_WIDE 4
#define DEFAULT_TILES_TALL 3

struct Bitmap
{
    char* name;//name of the bitmap for referencing quickly
    char* location;//file location
    HBITMAP image;//the actual HBITMAP object
};

struct MapTile
{
    int xPos;//x position in the map
    int yPos;//y position in the map
    struct Bitmap* bitmap; //We want a pointer to the original HBITMAP to avoid storing tons of duplicate HBITMAP objects
};



void LoadBitmaps(struct Bitmap*, int);//this function will load in all the bitmaps for use in the editor

void DrawMapTiles(struct MapTile**, int, int, int, int, float, HWND, HBITMAP);

void LoadTileImages();


#endif //RPG_ENGINE_MAPTILES_H
