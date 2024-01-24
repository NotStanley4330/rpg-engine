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


//consts for menu items
#define FILE_MENU_NEW 64
#define FILE_MENU_OPEN 65
#define FILE_MENU_EXIT 66
#define EDIT_WINDOW_BUTTON 67

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void AddMenus(HWND);
void AddControls(HWND);
void exitDialog(HWND);

void AddCatPicture(HWND);



//Menu handler
HMENU hMenu;

//window handlers
HWND hXCoord;

int main() {
    // Register window class
    const char CLASS_NAME[] = "SimpleWindowClass";

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
            CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800,

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

    return 0;
}

// Window procedure
//This is how we handle translate messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            AddMenus(hwnd);
            AddControls(hwnd);
            AddCatPicture(hwnd);

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
                case EDIT_WINDOW_BUTTON:
                {
                    //printf("Done correctly!\n");

                    //This code pulls the number from the hXCoord edit control and uses it to change the window title
                    int xLen = GetWindowTextLength(hXCoord);
                    char* xCoord = (char*)malloc((xLen + 1) * sizeof(char));
                    GetWindowText(hXCoord, xCoord, xLen + 1);
                    SetWindowText(hwnd, xCoord);
                    xCoord[xLen + 1] = '\0';


                    break;
                }
            }
            return 0;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
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
    AppendMenu(hEditSubMenu, MF_STRING, EDIT_WINDOW_BUTTON, "DO IT!");

    //The append calls are where we append top level menus to the bar
    AppendMenu(hMenu, MF_POPUP,(UINT_PTR)hFileMenu,"File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, "Edit");
    AppendMenu(hMenu, MF_STRING,1,"Help");


    //This is where the menus are bound to the menu
    SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd)
{
    CreateWindowEx(
            0,
            "Static",
            "Enter x coordinate here:",
            WS_VISIBLE | WS_CHILD,

            100,
            100,
            160,
            20,

            hwnd,
            NULL,
            NULL,
            NULL
            );

    hXCoord = CreateWindowEx(
            0,
            "Edit",
            "0",
            WS_VISIBLE | WS_CHILD | ES_NUMBER,

            160,
            120,
            80,
            20,

            hwnd,
            NULL,
            NULL,
            NULL

            );

}


void AddCatPicture(HWND hwnd)
{

}
