//
// Created by Michael Norris on 3/4/2024.
//
#include "maptiles.h"
#include <stdio.h>

#define WORLD_SCREEN_POS_X 350
#define WORLD_SCREEN_POS_Y 130

struct MapTile** currMapTiles;


//This function will be used to draw the tiles to physical area, including partial tiles.
//This will also be used for adding or taking away tiles when zooming in and out, with the goal to keep
//The current coords centered in the map and structuring everything around it.
void DrawMapTiles(struct MapTile** mapTiles, int width, int height, int worldPosX, int worldPosY,
        float worldZoom, HWND hwnd, HBITMAP backgroundBitmap)
{
    PAINTSTRUCT ps;
    // Adding in this stuff to test out bitblt
    BITMAP bitmap;
    HDC hdcMem;
    HGDIOBJ oldBitmap;
    HDC hdc = BeginPaint(hwnd, &ps);
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    // Draw hGameWorldView
    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, backgroundBitmap);
    GetObject(backgroundBitmap, sizeof(BITMAP), &bitmap);
    BitBlt(hdc, WORLD_SCREEN_POS_X, WORLD_SCREEN_POS_Y, bitmap.bmWidth, bitmap.bmHeight,
           hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);


    //code to determine the position of the upper left corner based on zoom level and worldPos
    int topLeftWorldPosX = worldPosX - ((width / 2) * (int)(DEFAULT_TILE_SIZE));
    int topLeftWorldPosY = worldPosY - ((height / 2) * (int)(DEFAULT_TILE_SIZE));

    //temp code to progtamatically populate the CurrTiles, eventually this will pull from
    //the total title to create a current tile set to render
    int tileXNum = topLeftWorldPosX / (int)(DEFAULT_TILE_SIZE);
    int tileYNum = topLeftWorldPosY / (int)(DEFAULT_TILE_SIZE);

    //populate our currMapTiles array

    //initialize it
    currMapTiles = (struct MapTile **)malloc(width * sizeof(struct MapTile *));
    for (int x = 0; x < width; x++)
    {
        currMapTiles[x] = (struct MapTile *)malloc(height * sizeof(struct MapTile));
    }
    //fill it
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            //this copies the tile over in its entirety, I may want to make this a pointer to
            //the existing tile in the future to avoid wholesale copies.
            currMapTiles[x][y] = mapTiles[tileXNum + x][tileYNum + y];

            //I also need to do some kind of error checking so that I don't just grab more tiles than exist properly
            //the move and zoom functions SHOULD catch this, but I also should just error check it here for safety
        }
    }


    //draw tiles programatically
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            hdcMem = CreateCompatibleDC(hdc);//Create a new memory DC for the current tile
            oldBitmap = SelectObject(hdcMem, currMapTiles[x][y].bitmap->image);

            //ignore error handling for now, just StrectchBlt with reckless abandon!!!
            StretchBlt(hdc, WORLD_SCREEN_POS_X + ((int)(DEFAULT_TILE_SIZE * worldZoom) * x),
                       WORLD_SCREEN_POS_Y + ((int)(DEFAULT_TILE_SIZE * worldZoom) * y),
                       DEFAULT_TILE_SIZE * worldZoom, DEFAULT_TILE_SIZE * worldZoom, hdcMem,
                       0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);
        }
    }

    EndPaint(hwnd, &ps);

    //free up all the memory for currMapTiles
    for (int x = 0; x < width; x++)
    {
        free(currMapTiles[x]);
    }
    free(currMapTiles);

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

        if (images[x].image == NULL)
        {
            // Failed to load image
            DWORD dwError = GetLastError(); // Optionally, you can get more information about the error
            LPVOID lpMsgBuf;

            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          NULL,
                          dwError,
                          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          (LPTSTR)&lpMsgBuf,
                          0,
                          NULL);

            // Print out the error message
            printf("Error loading image: %s\n", (LPCTSTR)lpMsgBuf);

            // Free the buffer
            LocalFree(lpMsgBuf);
        } else
        {

        }
    }
}


