//
// Created by Michael Norris on 3/4/2024.
//
#include "maptiles.h"

struct MapTile** currMapTiles;
int tilesWide = DEFAULT_TILES_WIDE;
int tilesTall = DEFAULT_TILES_TALL;

//This function will be used to draw the tiles to physical area, including partial tiles.
//This will also be used for adding or taking away tiles when zooming in and out, with the goal to keep
//The current coords centered in the map and structuring everything around it.
void DrawMapTiles(struct MapTile** mapTiles, int width, int height)
{

}

//Simply load in every bitmap using the given filenames and information
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
