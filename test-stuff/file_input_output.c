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

        for (int y = 0; y < numTilesY; y++)
        {
            if (x < 5 || y < 3)
            {
                tileName = "TILE_DIRT";
            }
            else if (x < 6)
            {
                tileName = "TILE_COAST";
            }
            else if (x < 10)
            {
                tileName = "TILE_OCEAN";
            }
            else if (x < 11)
            {
                tileName = "TILE_COAST_WEST";
            }
            else if (x < 12)
            {
                tileName = "TILE_DIRT";
            }

            if (y < 4 && y >= 3 && x >= 6 && x < 10)
            {
                tileName = "TILE_COAST_SOUTH";
            }
            else if (y < 12 && y >= 11 && x >= 6 && x < 10)
            {
                tileName = "TILE_COAST_NORTH";
            }
            else if (y >= 12 && x >= 6 && x < 10)
            {
                tileName = "TILE_DIRT";
            }
            //this is all gonna be replaced with actually reading in the file values of course
            mapTiles[x][y].xPos = x * DEFAULT_TILE_SIZE;
            mapTiles[x][y].yPos = y * DEFAULT_TILE_SIZE;
            //we need to find the bitmap associated with the name of the tile
            for (int z = 0; z < 7; z++)//TODO: Fix this to use the same size as the size of bitmaps
            {
                if (strcmp(tileName, bitmaps[z].name) == 0)
                {
                    mapTiles[x][y].bitmap = &bitmaps[z];
                }
            }

        }
    }

    return 0;
}

int WriteMapFile(char* fileName, struct Bitmap* bitmaps, struct MapTile** mapTiles, int numX, int numY)
{

}
