#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <process.h>

#include "main.h"
#include "window.h"
#include "sprloader.h"
#include "font.h"
#include "Events.h"
#include "definitions.h"

Window *g_window = NULL;
Screen *g_screen = NULL;
Font *g_font = NULL;
//Connection *g_connection = NULL;

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch (uMsg)                           // Check For Windows Messages
    {
        case WM_ACTIVATE:                  // Watch For Window Activate Message
        {
            if (!HIWORD(wParam))          // Check Minimization State
            {
                g_window->SetActive( true );           // Program Is Active
            }
            else
            {
                g_window->SetActive( false);          // Program Is No Longer Active
            }
            return 0;                     // Return To The Message Loop
        }

        case WM_SYSCOMMAND:                // Intercept System Commands
        {
            switch (wParam)               // Check System Calls
            {
                case SC_SCREENSAVE:      // Screensaver Trying To Start?
                case SC_MONITORPOWER:    // Monitor Trying To Enter Powersave?
                return 0;                // Prevent From Happening
            }
            break;
        }

        case WM_CLOSE:                     // Did We Receive A Close Message?
        {
            PostQuitMessage(0);           // Send A Quit Message
            return 0;
        }

        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            if((wParam >= 0) && (wParam <= 255))
            {
                KeyE e;
                e.Key =(EKEY_CODE)wParam;
                e.PressedDown = (bool)(0x0101-uMsg);

                WORD KeyAsc=0;
                BYTE allKeys[256];
                GetKeyboardState(allKeys);
                ToAscii((UINT)wParam,(UINT)lParam,allKeys,&KeyAsc,0);

                e.Shift = ((allKeys[VK_SHIFT] & 0x80)!=0);
                e.Alt = ((allKeys[VK_MENU] & 0x80)!=0);
                e.Control = ((allKeys[VK_CONTROL] & 0x80)!=0);
                e.Char = KeyAsc;
                g_screen->KeyEvent(&e);
            }
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            MouseE e;
            e.X = (short)LOWORD(lParam);
            e.Y = (short)HIWORD(lParam);
            e.Button = EMOUSE_NONE;
            e.State = EMOUSE_MOVE;
            e.param = wParam;
            g_screen->MouseEvent(&e);
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        {
            MouseE e;
            e.X = (short)LOWORD(lParam);
            e.Y = (short)HIWORD(lParam);
            e.Button = EMOUSE_LEFT;
            e.State = (uMsg-0x0201 == 0)?EMOUSE_BUTTON_DOWN:EMOUSE_BUTTON_UP;
            e.param = wParam;
            g_screen->MouseEvent(&e);
           break;
        }
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        {
            MouseE e;
            e.X = (short)LOWORD(lParam);
            e.Y = (short)HIWORD(lParam);
            e.Button = EMOUSE_RIGHT;
            e.State = (uMsg-0x0201 == 0)?EMOUSE_BUTTON_DOWN:EMOUSE_BUTTON_UP;
            e.param = wParam;
            g_screen->MouseEvent(&e);
            break;
        }

        case WM_SIZE:
        {
            /* Update the Render */
            g_window->Resize(LOWORD(lParam),HIWORD(lParam));     // LoWord=Width, HiWord=Height
            /* Update the screen context */
            g_screen->Resize(LOWORD(lParam),HIWORD(lParam));
            return 0;
        }
    }

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void ConnectionHandler( void *data )
{
    /*
    NetworkMessage msg;
    for (;;)
    {
        if (g_connection->IsConnected())
        {
            msg.ReadFromSocket(g_connection->GetSocket());
        }
        else
        {
            Sleep(500);
        }
    }
    */
}

//int main (HINSTANCE	hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int WINAPI WinMain(	HINSTANCE	hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG  msg;
    bool done = false;

    /* Checking if there is another client open */
    /* TODO: Find another way to block two clients */
    /*
    CreateMutex(NULL, FALSE, (char*)"1TEST1");

    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, "An instance of GAME_NAME is already running.", "Error", MB_OK);
        return EXIT_SUCCESS;
    }
    */
    /* Some checks if the computer has the basics we need */
    /*
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        MessageBox(NULL, "You need WinSocks with at least 2.2", "Error", MB_OK);
        return EXIT_SUCCESS;
    }
    */
    /* Initialize Fonts data */
    g_font = new Font();

    /* Maybe some code to check what kind of render should we open? */
    OpenGL opengl;
    g_window = &opengl;

    g_window->sprites = new Sprite[SPRITES_NUMBER];

    Sprite_Loader(g_window->sprites, "data/");

    /* There is only one client open, let's begin! */
    if (!g_window->Create("My Game",800,600,24))
    {
        return 0;
    }

    /* A new client is opened, only possible screen, eh? */
    g_screen = new LoginScreen();
    
    g_screen->Resize(800,600);

    /* Init the network thread */
    /*
    g_connection = new Connection();
    _beginthread(ConnectionHandler, 0, NULL);
    */
    while(!done)
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
        {
            if (msg.message==WM_QUIT)
            {
                done = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else if ( g_window->IsActive() )
        {
            g_window->DrawScene();
            g_window->Display();
        }
    }

    for ( int i = 0; i < SPRITES_NUMBER; i ++ )
    {
        if ( g_window->sprites[i].data )
        {
            delete g_window->sprites[i].data;
        }
    }
    delete g_window->sprites;

    g_window->Destroy();

    return (msg.wParam);
}
