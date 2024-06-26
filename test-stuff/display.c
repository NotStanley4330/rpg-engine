/*********************************************
 * File: display.c
 * Author: Michael Norris
 * Date: Jan-12-2024
 * Usage: This is merely a test file to play with
 * how to output some tiles to the screen and
 * other functions tbd (scaling,etc)
 *
 *
 *********************************************/

#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include "maptiles.h"
#include "file_input_output.h"


//consts for menu items
#define FILE_MENU_NEW 64
#define FILE_MENU_OPEN 65
#define FILE_MENU_EXIT 66
#define COORD_CONFIRM_BUTTON 67
#define LOAD_FILE_BUTTON 68
#define SAVE_FILE_BUTTON 69

//consts for map position buttons
#define X_COORD_INCREASE 20
#define X_COORD_DECREASE 21
#define Y_COORD_INCREASE 22
#define Y_COORD_DECREASE 23

//consts for zoom in and out buttons
#define ZOOM_OUT 24
#define ZOOM_IN 25

//consts for world screen position, as well as size
#define WORLD_SCREEN_POS_X 350
#define WORLD_SCREEN_POS_Y 130
#define WORLD_SCREEN_WIDTH_TILES 4
#define WORLD_SCREEN_HEIGHT_TILES 4


// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void AddMenus(HWND);
void AddControls(HWND);
void exitDialog(HWND);
int canZoomMove(float, int, int);

void loadButtonImages();

//globals - I know this is bad practice but screw passing around these values
int catPosX, catPosY, catWidth, catHeight;

//globals for mapLocation and zoom level
int worldPosX, worldPosY;
float worldZoom;

//Menu handler
HMENU hMenu;

//window handlers
HWND hXCoord, hYCoord;
HWND hConfCoords;

//filesave controls
HWND hFileName;
HWND hSaveFile;
HWND hLoadFile;

//window handlers for position buttons
HWND hDownButton, hUpButton, hLeftButton, hRightButton;
HWND hMinusButton, hPlusButton;


//Image Handlers
HBITMAP hCatImage;
HBITMAP hDownArrow, hUpArrow, hLeftArrow, hRightArrow;
HBITMAP hMinus, hPlus;
HBITMAP hGameWorldView;

//CAT
HWND hCat;
int numBitmaps = 7;

//ALL THE BITMAPS
struct Bitmap bitmaps[7];
//ALL THE TILES
struct MapTile** mapTiles;
//dirty globals so I can easily keep track of the number of tiles
int numTilesX, numTilesY;


int main() {
    // Register window class
    const char CLASS_NAME[] = "SimpleWindowClass";

    //initialize our cat positions
    catPosX = 1000;
    catPosY = 250;
    catWidth = 150;
    catHeight = 150;

    //initialize our world zoom options
    //eventually we want to have these loaded in from a custom file so that we can preserve progress between sessions
    worldPosX = 256;
    worldPosY = 512;
    worldZoom = 2.0;

    //here we should load in some bitmaps


    //load in all the tiles
    numTilesX = 20;
    numTilesY = 20;
    //lets set up the mapTiles array
    mapTiles = (struct MapTile **)malloc(numTilesX * sizeof(struct MapTile *));
    for (int x = 0; x < numTilesX; x++)
    {
        mapTiles[x] = (struct MapTile *)malloc(numTilesY * sizeof(struct MapTile));
    }
    ReadMapFile("C:\\Users\\starw\\CLionProjects\\rpg-engine\\mapfiles\\mapfile_1_working.rpm",bitmaps, mapTiles,
                numTilesX, numTilesY, &numBitmaps);
//    WriteMapFile("C:\\Users\\starw\\CLionProjects\\rpg-engine\\mapfiles\\mapfile_1.rpm", bitmaps, mapTiles,
//                 numTilesX, numTilesY, numBitmaps);




    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
            0,                          // Optional window styles
            CLASS_NAME,                 // Window class
            "Example Display",            // Window text
            WS_OVERLAPPEDWINDOW,        // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, 1400, 800,

            NULL,       // Parent window
            NULL,       // Menu
            GetModuleHandle(NULL), // Instance handle
            NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Display the window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // Message loop
    //This is where we handle any event messages sent by the OS (ie Mouse Clicks)
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //we done gone and malloced a TON of memory so lets free it all
    for (int x = 0; x < numTilesX; x++)
    {
        free(mapTiles[x]);
    }
    free(mapTiles);

    return 0;
}

// Window procedure
//This is how we handle translate messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            loadButtonImages();
            AddMenus(hwnd);
            AddControls(hwnd);

            //we are gonna test having a bitmap that displays the game world
            hGameWorldView = (HBITMAP)LoadImage(
                    NULL,
                    "C:\\Users\\starw\\CLionProjects\\rpg-engine\\test-stuff\\world_background.bmp",
                    IMAGE_BITMAP,
                    400,
                    400,
                    LR_LOADFROMFILE
            );
            //let's load in some test tiles so that we can work on bitblt

            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND://this is called when buttons on menus are clicked
            switch(wParam)//this is where we do whatever the button does
            {
                case 1:
                    MessageBeep(MB_OK);
                    break;
                case FILE_MENU_NEW:
                    MessageBeep(MB_ICONERROR);
                    break;
                case FILE_MENU_OPEN:
                    break;//currently does nothing
                case FILE_MENU_EXIT:
                    exitDialog(hwnd);
                    // DO nothing if the user cancelled
                    break;
                case COORD_CONFIRM_BUTTON:
                {
                    //pulls the number from the hXCoord edit control
                    int xLen = GetWindowTextLength(hXCoord);
                    char* xCoord = (char*)malloc((xLen + 1) * sizeof(char));
                    GetWindowText(hXCoord, xCoord, xLen + 1);
                    int newWorldPosX = atoi(xCoord);
                    //let's do the same for the y coord control
                    int yLen = GetWindowTextLength(hYCoord);
                    char* yCoord = (char*)malloc((yLen + 1) * sizeof(char));
                    GetWindowText(hYCoord, yCoord, yLen + 1);
                    int newWorldPosY = atoi(yCoord);
                    if (canZoomMove(worldZoom, newWorldPosX, newWorldPosY))
                    {
                        worldPosX = newWorldPosX;
                        worldPosY = newWorldPosY;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
                case LOAD_FILE_BUTTON:
                {
                    int nameLen = GetWindowTextLength(hFileName);
                    char* fileName = (char*)malloc((nameLen + 1) * sizeof(char));
                    GetWindowText(hFileName, fileName, nameLen + 1);
                    char* fileNameFull;
                    fileNameFull = "C:\\Users\\starw\\CLionProjects\\rpg-engine\\mapfiles\\****************************";
                    char fileNameFinal[256];
                    int numCharFN = 0;
                    for (int x = 0; x < 256; x++) {
                        {
                            if (fileNameFull[x] == '*')
                            {
                                fileNameFinal[x] = fileName[numCharFN];
                                numCharFN++;
                            } else if (fileNameFull[x] == '\0' || numCharFN == nameLen)
                            {
                                fileNameFinal[x] = '\0';
                                break;
                            } else
                            {
                                fileNameFinal[x] = fileNameFull[x];
                            }
                        }
                    }
                    //Now we need to read in the map file and invalidate the old rectangle
                    ReadMapFile(fileNameFinal, bitmaps, mapTiles, numTilesX, numTilesY, &numBitmaps);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
                case SAVE_FILE_BUTTON:
                {
                    int nameLen = GetWindowTextLength(hFileName);
                    char* fileName = (char*)malloc((nameLen + 1) * sizeof(char));
                    GetWindowText(hFileName, fileName, nameLen + 1);

                    char* fileNameFull = "C:\\Users\\starw\\CLionProjects\\rpg-engine\\mapfiles"
                                   "\\******************************";
                    char fileNameFinal[256];
                    int numCharFN = 0;
                    for (int x = 0; x < 256; x++)
                    {
                        {
                            if (fileNameFull[x] == '*')
                            {
                                fileNameFinal[x] = fileName[numCharFN];
                                numCharFN++;
                            }
                            else if (fileNameFull[x] == '\0' || numCharFN == nameLen)
                            {
                                fileNameFinal[x] = '\0';
                                break;
                            }
                            else
                            {
                                fileNameFinal[x] = fileNameFull[x];
                            }
                        }
                    }
                    WriteMapFile(fileNameFinal, bitmaps, mapTiles, numTilesX, numTilesY, numBitmaps);

                    break;
                }
                case Y_COORD_INCREASE:
                {
                    //move over by one tile for simplicity
                    int newWorldPosY = worldPosY + (int)(DEFAULT_TILE_SIZE * (2 / worldZoom));
                    if (canZoomMove(worldZoom, worldPosX, newWorldPosY))
                    {
                        worldPosY = newWorldPosY;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
                case Y_COORD_DECREASE:
                {
                    int newWorldPosY = worldPosY - (int)(DEFAULT_TILE_SIZE * (2 / worldZoom));
                    if (canZoomMove(worldZoom, worldPosX, newWorldPosY))
                    {
                        worldPosY = newWorldPosY;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
                case X_COORD_INCREASE:
                {
                    int newWorldPosX = worldPosX + (int)(DEFAULT_TILE_SIZE * (2 / worldZoom));
                    if (canZoomMove(worldZoom, newWorldPosX, worldPosY))
                    {
                        worldPosX = newWorldPosX;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
                case X_COORD_DECREASE:
                {
                    //move left by one tile at normal zoom (or zoom dependent number) if possible
                    int newWorldPosX = worldPosX - (int)(DEFAULT_TILE_SIZE * (2 / worldZoom));
                    if (canZoomMove(worldZoom, newWorldPosX, worldPosY))
                    {
                        worldPosX = newWorldPosX;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
                case ZOOM_OUT:
                {
                    //decrease tile size in view by half if possible
                    float newWorldZoom = worldZoom / 2;
                    if(canZoomMove(newWorldZoom, worldPosX, worldPosY))
                    {
                        worldZoom = newWorldZoom;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }

                    break;
                }
                case ZOOM_IN:
                {
                    //increase tile size by 2 in view if possible
                    float newWorldZoom = worldZoom * 2;
                    if(canZoomMove(newWorldZoom, worldPosX, worldPosY))
                    {
                        worldZoom = newWorldZoom;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
            }
            return 0;
        case WM_PAINT:
        {
            //NOTE: Use StretchBlt for ease of resizing tiles and textures
            int tilesHigh = WORLD_SCREEN_WIDTH_TILES / (worldZoom / 2);
            int tilesWide = WORLD_SCREEN_HEIGHT_TILES / (worldZoom / 2);
            DrawMapTiles(mapTiles, tilesWide, tilesHigh, worldPosX, worldPosY, worldZoom, hwnd, bitmaps[3].image);

        }
            return 0;
        case WM_CLOSE:
            exitDialog(hwnd);
            // If it gets to here the user cancelled exiting, do nothing
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//This function is used to create the exit dialog and handle destruction if necessary
//Probably expand this to have a save and exit option in the future
void exitDialog(HWND hwnd)
{
    if (MessageBox(hwnd, "Really quit?", "Quit dialog", MB_OKCANCEL) == IDOK)
    {
        WriteMapFile("C:\\Users\\starw\\CLionProjects\\rpg-engine\\mapfiles\\mapfile_1_empty.rpm", bitmaps, mapTiles,
                     numTilesX, numTilesY, numBitmaps);
        DestroyWindow(hwnd);
    }
}

//This function is used for creating all the menus at the top bar
void AddMenus(HWND hwnd)
{
    hMenu = CreateMenu();

    //dropdown menus
    HMENU hFileMenu = CreateMenu();
    HMENU hEditMenu = CreateMenu();
    HMENU hEditSubMenu = CreateMenu();

    //Append items to file menu
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_OPEN, "Open");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

    //Append items to the Edit menu
    AppendMenu(hEditMenu, MF_POPUP, (UINT_PTR)hEditSubMenu, "Window Name");

    //Append to the Edit submenu
    AppendMenu(hEditSubMenu, MF_STRING, 0, "DO IT!"); //Should not change the window name now

    //The append calls are where we append top level menus to the bar
    AppendMenu(hMenu, MF_POPUP,(UINT_PTR)hFileMenu,"File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, "Edit");
    AppendMenu(hMenu, MF_STRING,1,"Help");


    //This is where the menus are bound to the menu
    SetMenu(hwnd, hMenu);
}

//This function creates the actual buttons
void AddControls(HWND hwnd)
{
    //Create the static text for the coord manual controls
    CreateWindowEx(
            0,
            "Static",
            "x:",
            WS_VISIBLE | WS_CHILD,

            100,
            100,
            20,
            20,

            hwnd,
            NULL,
            NULL,
            NULL
            );

    CreateWindowEx(
            0,
            "Static",
            "y:",
            WS_VISIBLE | WS_CHILD,

            160,
            100,
            20,
            20,

            hwnd,
            NULL,
            NULL,
            NULL
    );

    //create the x and Y controls
    hXCoord = CreateWindowEx(
            0,
            "Edit",
            "256",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,

            100,
            120,
            50,
            20,

            hwnd,
            NULL,
            NULL,
            NULL

            );

    hYCoord = CreateWindowEx(
            0,
            "Edit",
            "512",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,

            160,
            120,
            50,
            20,

            hwnd,
            NULL,
            NULL,
            NULL

    );

    hFileName = CreateWindowEx(
            0,
            "Edit",
            "mapfile_1.rpm",
            WS_VISIBLE | WS_CHILD | WS_BORDER,

            100,
            250,
            150,
            20,

            hwnd,
            NULL,
            NULL,
            NULL

    );

    hSaveFile = CreateWindowEx(
            0,
            "Button",
            "Save File",
            WS_VISIBLE | WS_CHILD,

            120,
            305,
            120,
            20,

            hwnd,
            (HMENU)SAVE_FILE_BUTTON,
            NULL,
            NULL
    );
    hSaveFile = CreateWindowEx(
            0,
            "Button",
            "Load File",
            WS_VISIBLE | WS_CHILD,

            120,
            280,
            120,
            20,

            hwnd,
            (HMENU)LOAD_FILE_BUTTON,
            NULL,
            NULL
    );



    hConfCoords = CreateWindowEx(
                0,
                "Button",
                "Change Coords",
                WS_VISIBLE | WS_CHILD,

                120,
                160,
                120,
                20,

                hwnd,
                (HMENU)COORD_CONFIRM_BUTTON,
                NULL,
                NULL
            );


    hDownButton = CreateWindowEx(
            0,
            "Button",
            NULL,
            WS_VISIBLE | WS_CHILD | BS_BITMAP,

            550,
            650,
            40,
            60,

            hwnd,
            (HMENU)Y_COORD_INCREASE,
            NULL,
            NULL
    );

    hUpButton = CreateWindowEx(
            0,
            "Button",
            NULL,
            WS_VISIBLE | WS_CHILD | BS_BITMAP,

            550,
            50,
            40,
            60,

            hwnd,
            (HMENU)Y_COORD_DECREASE,
            NULL,
            NULL
    );

    hRightButton = CreateWindowEx(
            0,
            "Button",
            NULL,
            WS_VISIBLE | WS_CHILD | BS_BITMAP,

            950,
            300,
            60,
            40,

            hwnd,
            (HMENU)X_COORD_INCREASE,
            NULL,
            NULL
    );

    hLeftButton = CreateWindowEx(
            0,
            "Button",
            NULL,
            WS_VISIBLE | WS_CHILD | BS_BITMAP,

            250,
            300,
            60,
            40,

            hwnd,
            (HMENU)X_COORD_DECREASE,
            NULL,
            NULL
    );

    hMinusButton = CreateWindowEx(
            0,
            "Button",
            NULL,
            WS_VISIBLE | WS_CHILD | BS_BITMAP,

            450,
            665,
            60,
            30,

            hwnd,
            (HMENU)ZOOM_OUT,
            NULL,
            NULL
    );

    hPlusButton = CreateWindowEx(
            0,
            "Button",
            NULL,
            WS_VISIBLE | WS_CHILD | BS_BITMAP,

            650,
            665,
            30,
            30,

            hwnd,
            (HMENU)ZOOM_IN,
            NULL,
            NULL
    );




    //send messages to the controllers
    SendMessage(hCat, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCatImage);
    SendMessage(hDownButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDownArrow);
    SendMessage(hUpButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hUpArrow);
    SendMessage(hRightButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hRightArrow);
    SendMessage(hLeftButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLeftArrow);
    SendMessage(hMinusButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hMinus);
    SendMessage(hPlusButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hPlus);

}

//This function loads in all the bitmaps for buttons
void loadButtonImages()
{

    //May require a 24 bit depth bitmap?!?
    /********************************************************************************************************
    * Will eventually want to replace this cat logic because I don't think LoadImage supports transparency  *
    * Also think that It would be impractical to render the game world using this method, will probably     *
    * want to use BitBlt and custom load in the graphics somehow. Will further research what this requires. *
    * I also want to probably put all that functionality in that file, this file should be purely for       *
    * window controls for the main window. If I create sub-windows I probably want their control broken out *
    *********************************************************************************************************/

    hDownArrow = (HBITMAP)LoadImage(
            NULL,
            "C:\\Users\\starw\\CLionProjects\\rpg-engine\\test-stuff\\down_arrow.bmp",
            IMAGE_BITMAP,
            60,
            60,
            LR_LOADFROMFILE
    );

    hUpArrow = (HBITMAP)LoadImage(
            NULL,
            "C:\\Users\\starw\\CLionProjects\\rpg-engine\\test-stuff\\up_arrow.bmp",
            IMAGE_BITMAP,
            60,
            60,
            LR_LOADFROMFILE
    );

    hRightArrow = (HBITMAP)LoadImage(
            NULL,
            "C:\\Users\\starw\\CLionProjects\\rpg-engine\\test-stuff\\right_arrow.bmp",
            IMAGE_BITMAP,
            60,
            60,
            LR_LOADFROMFILE
    );

    hLeftArrow = (HBITMAP)LoadImage(
            NULL,
            "C:\\Users\\starw\\CLionProjects\\rpg-engine\\test-stuff\\left_arrow.bmp",
            IMAGE_BITMAP,
            60,
            60,
            LR_LOADFROMFILE
    );

    hMinus = (HBITMAP)LoadImage(
            NULL,
            "C:\\Users\\starw\\CLionProjects\\rpg-engine\\test-stuff\\minus_button.bmp",
            IMAGE_BITMAP,
            30,
            15,
            LR_LOADFROMFILE
    );
    hPlus = (HBITMAP)LoadImage(
            NULL,
            "C:\\Users\\starw\\CLionProjects\\rpg-engine\\test-stuff\\plus_button.bmp",
            IMAGE_BITMAP,
            30,
            30,
            LR_LOADFROMFILE
    );


}

//this function will be used to check whether a zoom or x/y camera movement should be allowed or not
//it will return 1 if allowed, and zero if it is not
//The function accepts up to three values but only one may be being changed, so the calling function should feed
//the old values if they are not being updated
int canZoomMove(float newZoomValue, int newXPos, int newYPos)
{
    int tilesHigh = WORLD_SCREEN_WIDTH_TILES / (newZoomValue / 2);
    int tilesWide = WORLD_SCREEN_HEIGHT_TILES / (newZoomValue / 2);
    int topLeftWorldPosX = newXPos - ((tilesWide / 2) * (int)(DEFAULT_TILE_SIZE));
    int topLeftWorldPosY = newYPos - ((tilesHigh / 2) * (int)(DEFAULT_TILE_SIZE));
    int tileXNumTopLeft = topLeftWorldPosX / (int)(DEFAULT_TILE_SIZE);
    int tileYNumTopLeft = topLeftWorldPosY / (int)(DEFAULT_TILE_SIZE);

    //let's also calculate where the bottom right tile is so that
    //we can check if it is out of bounds
    int bottomRightWorldPosX = newXPos + ((tilesWide / 2) * (int)(DEFAULT_TILE_SIZE));
    int bottomRightWorldPosY = newYPos + ((tilesHigh / 2) * (int)(DEFAULT_TILE_SIZE));
    int tileXNumBottomRight = bottomRightWorldPosX / (int)(DEFAULT_TILE_SIZE);
    int tileYNumBottomRight = bottomRightWorldPosY / (int)(DEFAULT_TILE_SIZE);

    //if the zoom will extend the view port further than the range of map tiles in any direction,
    //disallow the zoom by returning 0
    if (tileXNumTopLeft < 0 || tileYNumTopLeft < 0)
    {
        return 0;
    }
    else if (tileXNumBottomRight >= numTilesX || tileYNumBottomRight >= numTilesY)
    {
        return 0;
    }
    else if (tilesWide <= 0 ||tilesHigh <= 0)
    {
        //also disallow the zoom if the screen area would be smaller than a single tile
        return 0;
    }
    else
    {
        return 1;
    }

}

