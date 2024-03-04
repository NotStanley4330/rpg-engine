//
// Created by starw on 3/4/2024.
//
#include <windows.h>
#include <winuser.h>

#ifndef RPG_ENGINE_MAPTILES_H
#define RPG_ENGINE_MAPTILES_H

#define DEFAULT_TILE_SIZE 128

struct MapTile
{
    int xPos;
    int yPos;
    HBITMAP bitmap;
};

void DrawMapTiles(struct MapTile**, int, int);


#endif //RPG_ENGINE_MAPTILES_H
