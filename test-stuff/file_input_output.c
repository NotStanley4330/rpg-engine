//
// Created by Michael Norris on 3/4/2024.
// Function: read in map files with all of their relevant details
//

#include <stdio.h>
#include "file_input_output.h"

//this function will read in the map file and save all the data to the necessary spots
int ReadMapFile(char* fileName, struct Bitmap* bitmaps, struct MapTile** mapTiles, int numX, int numY, int* numBitmaps)
{
    //For now this is a dummy function that just creates a massive array of bitmaps
    int numTilesX = numX;
    int numTilesY = numY;



    char* tileName = "NULL";



    FILE *file_pointer;
    char line[256];
    file_pointer = fopen(fileName, "r");
    if (file_pointer == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    //First we need to check that this is the right kind of file we are reading in
    fgets(line, 256, file_pointer);
    if (strcmp(line, "RPGMAPFILE\n"))
    {
        printf("NOT A RPGMAPFILE!!\n");
        return 1;
    }
    //we also want to check that we have the bitmap section
    fgets(line, 256, file_pointer);
    if (strcmp(line, "BEGIN BITMAP SECTION:\n"))
    {
        printf("BITMAP SECTION MISSING!!!\n");
        return 1;
    }
    else
    {
        //if all is good read in the bitmaps
        int inBitmaps = 1;
        int bitmapNum = 0;
        while (inBitmaps)
        {
            //This gives us the tile num
            fgets(line, 256, file_pointer);
            //we wanna check if we have hit the end of the bitmap section
            if (!strcmp(line, "END BITMAP SECTION\n"))
            {
                inBitmaps = 0;
                //numBitmaps = &bitmapNum;
                continue;
            }
            //this gives us the bitmap name
            fgets(line, 256, file_pointer);
            fgets(line, 256, file_pointer);
            char tempName[256];
            for (int c = 0; c < 256; c++)
            {
                if (line[c] != '\n' && line[c] != '\0')
                {
                    tempName[c] = line[c];
                }
                else
                {
                    tempName[c] = '\0';
                    break;
                }
            }
            bitmaps[bitmapNum].name = strdup(tempName);
            //this gives us the bitmap location
            fgets(line, 256, file_pointer);
            fgets(line, 256, file_pointer);
            char tempLocation[256];
            for (int c = 0; c < 256; c++)
            {
                if (line[c] != '\n' && line[c] != '\0')
                {
                    tempLocation[c] = line[c];
                }
                else
                {
                    tempLocation[c] = '\0';
                    break;
                }
            }
            bitmaps[bitmapNum].location = strdup(tempLocation);
            bitmapNum++;



        }
        LoadBitmaps(bitmaps, 7);
    }

    //Now we want to read in the tile data
    fgets(line, 256, file_pointer);
    if (strcmp(line, "BEGIN TILE INFO SECTION:\n"))
    {
        printf("Missing Title Info Section!\n");
        return 1;
    }
    else
    {
        for (int x = 0; x < numTilesX; x++)
        {
            //Read in the tile row
            fgets(line, 256, file_pointer);
            for (int y = 0; y < numTilesY; y++)
            {
                //read in the tile num
                fgets(line, 256, file_pointer);
                //type header
                fgets(line, 256, file_pointer);
                //type info
                fgets(line, 256, file_pointer);
                char tempType[256];
                for (int c = 0; c < 256; c++)
                {
                    if (line[c] != '\n' && line[c] != '\0')
                    {
                        tempType[c] = line[c];
                    }
                    else
                    {
                        tempType[c] = '\0';
                        break;
                    }
                }

                //this is where we place all the tiles
                mapTiles[x][y].xPos = x * DEFAULT_TILE_SIZE;
                mapTiles[x][y].yPos = y * DEFAULT_TILE_SIZE;
                //we need to find the bitmap associated with the name of the tile
                for (int z = 0; z < 7; z++)
                {
                    if (strcmp(tempType, bitmaps[z].name) == 0)
                    {
                        mapTiles[x][y].bitmap = &bitmaps[z];
                    }
                }



            }
        }
    }

    return 0;
}

int WriteMapFile(char* fileName, struct Bitmap* bitmaps, struct MapTile** mapTiles, int numX, int numY, int numBitmaps)
{
    FILE *file_pointer;
    file_pointer = fopen(fileName, "w");
    if (file_pointer == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    fprintf(file_pointer, "RPGMAPFILE\n");
    fprintf(file_pointer, "BEGIN BITMAP SECTION:\n");
    for (int x = 0; x < numBitmaps; x++)
    {
        fprintf(file_pointer, "BITMAP NUM %i\n", x);
        fprintf(file_pointer, "NAME\n");
        fprintf(file_pointer, "%s\n", bitmaps[x].name);
        fprintf(file_pointer, "LOCATION\n");
        fprintf(file_pointer, "%s\n", bitmaps[x].location);
    }
    fprintf(file_pointer, "END BITMAP SECTION\n");
    fprintf(file_pointer, "BEGIN TILE INFO SECTION:\n");
    for (int x = 0;  x < numX; x++)
    {
        fprintf(file_pointer, "TILE ROW %i:\n", x);
        for (int y = 0; y < numY; y++)
        {
            fprintf(file_pointer, "TILE NUM %i:\n", y);
            fprintf(file_pointer, "TYPE:\n");
            fprintf(file_pointer, "%s\n", mapTiles[x][y].bitmap->name);
        }
    }




    fclose(file_pointer);
    printf("File saved at location: %s\n", fileName);
    return 0;
}
