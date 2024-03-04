//
// Created by starw on 3/4/2024.
//
#include "maptiles.h"


void DrawMapTiles(struct MapTile** mapTiles, int width, int height)
{

}

void LoadBitmaps(struct Bitmap* images, int numBitmaps)
{
    for(int x = 0; x < numBitmaps; x++)
    {
        images[x].image = (HBITMAP)LoadImage(
                NULL,
                images[x].location,
                IMAGE_BITMAP,
                DEFAULT_TILE_SIZE,
                DEFAULT_TILE_SIZE,
                LR_LOADFROMFILE
        );
    }
}
