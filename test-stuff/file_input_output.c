//
// Created by Michael Norris on 3/4/2024.
// Function: read in map files with all of their relevant details
//

#include <stdio.h>
#include "file_input_output.h"

//this function will read in the map file and save all the data to the necessary spots
int ReadMapFile(char* fileName, struct Bitmap* bitmaps, struct MapTile** mapTiles, int numX, int numY)
{
    //For now this is a dummy function that just creates a massive array of bitmaps
    int numTilesX = numX;
    int numTilesY = numY;



    char* tileName = "NULL";
    for (int x = 0; x < numTilesX; x++)
    {
        if (x < 5)
        {
            tileName = "TILE_DIRT";
        }
        else if (x < 6)
        {
            tileName = "TILE_COAST";
        }
        else
        {
            tileName = "TILE_OCEAN";
        }
        for (int y = 0; y < numTilesY; y++)
        {
            //this is all gonna be replaced with actually reading in the file values of course
            mapTiles[x][y].xPos = x * DEFAULT_TILE_SIZE;
            mapTiles[x][y].yPos = y * DEFAULT_TILE_SIZE;
            //we need to find the bitmap associated with the name of the tile
            for (int z = 0; z < 3; z++)
            {
                if (strcmp(tileName, bitmaps[z].name))
                {
                    mapTiles[x][y].bitmap = &bitmaps[z];
                }
            }

        }
    }

    return 0;
}
