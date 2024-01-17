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

//consts for menu items
#define FILE_MENU_NEW 64
#define FILE_MENU_OPEN 65

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void AddMenus(HWND);

//Menu handler
HMENU hMenu;

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
            if (MessageBox(hwnd, "Really quit?", "Quit dialog", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd);
            }
            // Else: User canceled. Do nothing.
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AddMenus(HWND hwnd)
{
    hMenu = CreateMenu();

    //dropdown menus
    HMENU hFileMenu = CreateMenu();

    //Append items to file menu
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_OPEN, "Open");

    //The append calls are where we append top level menus to the bar
    AppendMenu(hMenu, MF_POPUP,(UINT_PTR)hFileMenu,"File");
    AppendMenu(hMenu, MF_STRING,1,"Help");


    //This is where the menus are bound to the menu
    SetMenu(hwnd, hMenu);
}
