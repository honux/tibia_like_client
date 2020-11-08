#include "definitions.h"
#include "opengl.h"
#include "GUI_element.h"
#include <string.h>
#include "font.h"

extern Screen *g_screen;
extern Font *g_font;

OpenGL::OpenGL ( void )
{
}

bool OpenGL::Create( char *name, int _width, int _height, int _bits)
{
     m_width             = _width;
     m_height            = _height;
     GLuint              PixelFormat;	// Holds The Results After Searching For A Match
     WNDCLASS            wc;			// Windows Class Structure
     DWORD               dwExStyle;	// Window Extended Style
     DWORD               dwStyle;		// Window Style
     RECT                WindowRect;	// Grabs Rectangle Upper Left / Lower Right Values
     WindowRect.left     = (long)0;	// Set Left Value To 0
     WindowRect.right    = (long)m_width;	// Set Right Value To Requested Width
     WindowRect.top      = (long)0;	// Set Top Value To 0
     WindowRect.bottom   = (long)m_height;// Set Bottom Value To Requested Height

     hInstance           = GetModuleHandle(NULL);				// Grab An Instance For Our Window
     wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
     wc.lpfnWndProc      = (WNDPROC) WndProc;				// WndProc Handles Messages
     wc.cbClsExtra       = 0;								// No Extra Window Data
     wc.cbWndExtra       = 0;								// No Extra Window Data
     wc.hInstance        = hInstance;						// Set The Instance
     wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);		// Load The Default Icon
     wc.hCursor          = LoadCursor(NULL, IDC_ARROW);		// Load The Arrow Pointer
     wc.hbrBackground    = NULL;							// No Background Required For GL
     wc.lpszMenuName     = NULL;							// We Don't Want A Menu
     wc.lpszClassName    = "OpenGL";						// Set The Class Name

     /* Attempt To Register The Window Class */
     if (!RegisterClass(&wc))
     {
          MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
          return false;
     }
	
     dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
     dwStyle   = WS_OVERLAPPEDWINDOW;				// Windows Style

     AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);// Adjust Window To True Requested Size

     /* Create The Window */
     if (!(hWnd=CreateWindowEx( dwExStyle,                            // Extended Style For The Window
                                "OpenGL",                             // Class Name
                                name,                                 // Window Title
                                dwStyle |                             // Defined Window Style
                                WS_CLIPSIBLINGS |                     // Required Window Style
                                WS_CLIPCHILDREN,                      // Required Window Style
                                0, 0,                                 // Window Position
                                WindowRect.right-WindowRect.left,     // Calculate Window Width
                                WindowRect.bottom-WindowRect.top,     // Calculate Window Height
                                NULL,                                 // No Parent Window
                                NULL,                                 // No Menu
                                hInstance,                            // Instance
                                NULL)))	                              // Dont Pass Anything To WM_CREATE
     {
          Destroy(); // Reset The Display
          MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
          return false;	// Return FALSE
     }
     
     static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
     {
          sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
          1,										// Version Number
          PFD_DRAW_TO_WINDOW |						// Format Must Support Window
          PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
          PFD_DOUBLEBUFFER,							// Must Support Double Buffering
          PFD_TYPE_RGBA,								// Request An RGBA Format
          bits,									// Select Our Color Depth
          0, 0, 0, 0, 0, 0,							// Color Bits Ignored
          0,										// No Alpha Buffer
          0,										// Shift Bit Ignored
          0,										// No Accumulation Buffer
          0, 0, 0, 0,								// Accumulation Bits Ignored
          16,										// 16Bit Z-Buffer (Depth Buffer)  
          0,										// No Stencil Buffer
          0,										// No Auxiliary Buffer
          PFD_MAIN_PLANE,							// Main Drawing Layer
          0,										// Reserved
          0, 0, 0									// Layer Masks Ignored
     };
	
     /* Running bunch of tests, so we will know better where the program got screwed */

     /* Did We Get A Device Context? */
     if (!(this->hDC=GetDC(this->hWnd)))
     {
          Destroy();
          MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
          return false;
     }
     /* Did Windows Find A Matching Pixel Format? */
     if (!(PixelFormat=ChoosePixelFormat(this->hDC,&pfd)))
     {
          Destroy();
          MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
          return false;
     }
     /* Are We Able To Set The Pixel Format? */
     if(!SetPixelFormat(hDC,PixelFormat,&pfd))
     {
          Destroy();
          MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
          return false;
     }
     /* Are We Able To Get A Rendering Context? */
     if (!(this->hRC=wglCreateContext(this->hDC)))
     {
          Destroy();
          MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
          return false;
     }
     /* Try To Activate The Rendering Context */
     if(!wglMakeCurrent(this->hDC,this->hRC))
     {
          Destroy();
          MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
          return false;
     }

     ShowWindow(this->hWnd,SW_SHOW);    // Show The Window
     SetForegroundWindow(hWnd);         // Slightly Higher Priority
     SetFocus(this->hWnd);              // Sets Keyboard Focus To The Window
     
     Resize(m_width, m_height);      // Set Up Our Perspective GL Screen
     glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
     glEnable(GL_TEXTURE_2D);                               // Enable Textures
     glEnable(GL_ALPHA_TEST);
     glAlphaFunc(GL_GREATER, 0.1);

     _GenerateTextures();
     _GenerateFont();

     return true;

}

void OpenGL::DrawScene ( void )
{
    unsigned now = GetTickCount();
    static unsigned last = GetTickCount();
    static unsigned current_fps;
    static unsigned last_fps;

    if ( now - last > 1000 )
    {
        last = now;
        last_fps = current_fps+1;
        current_fps = 0;
    }
    current_fps++;

    for ( std::list<GUI_Element*>::iterator it = g_screen->m_screenElement.begin(); it != g_screen->m_screenElement.end(); it++)
    {
        (*it)->Render();
    }

    if (g_screen->m_displayFps)
    {
        char buffer[100];
        sprintf(buffer, "FPS: %d", last_fps);
        RenderText( Vector ( 0, 0 ), buffer, ECOLOR( 255, 255, 0 ) );
    }


    /*
    GUI_Message message ( 249, 150, Vector(500, 500), "test without new line", "About" );
    message.Render();

    GUI_Rectangle background(800, 600, Vector(0,0), 1, Vector2(0.0, (424.0f/1024.0f), (800.0f/1024.0f), 1.0));
    background.Render();

    GUI_Button button(40, 50, Vector(500, 500), "Name", NULL);
    button.Render();
     */
}

void OpenGL::Resize ( int _width, int _height )
{
     m_width       = _width;
     m_height      = _height;

     if (m_height==0)										// Prevent A Divide By Zero By
     {
          m_height=1;										// Making Height Equal One
     }

     glViewport(0,0,m_width,m_height);						// Reset The Current Viewport

     glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
     glLoadIdentity();									// Reset The Projection Matrix
     glOrtho(0.0f, m_width, m_height, 0.0f, -1.0f, 1.0f);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();									// Reset The Modelview Matrix
}

void OpenGL::Display ( void )
{
    SwapBuffers(hDC);
}

void OpenGL::Destroy ( void )
{
     /* Do We Have A Rendering Context? */
     if (this->hRC)
     {
          /* Are We Able To Release The DC And RC Contexts? */
          if (!wglMakeCurrent(NULL,NULL))
          {
               MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
          }
          /* Are We Able To Delete The RC? */
          if (!wglDeleteContext(hRC))
          {
               MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
          }
          hRC=NULL;
     }
     /* Are We Able To Release The DC */
     if (this->hDC && !ReleaseDC(this->hWnd,this->hDC))
     {
          MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
          hDC=NULL;
     }
     /* Are We Able To Destroy The Window? */
     if (this->hWnd && !DestroyWindow(this->hWnd))
     {
          MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
          hWnd=NULL;
     }
     /* Are We Able To Unregister Class */
     if (!UnregisterClass("OpenGL",this->hInstance))
     {
          MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
          hInstance=NULL;
     }
}

void OpenGL::_GenerateTextures ( void )
{
     glGenTextures(SPRITES_NUMBER, &textures[0]);

     for ( int i = 0; i < SPRITES_NUMBER; i++ )
     {
          glBindTexture(GL_TEXTURE_2D, textures[i]);

          if ( i < 3 )
          {
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          }
          else
          {
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          }

          glTexImage2D(GL_TEXTURE_2D, 0, 4, sprites[i].width, sprites[i].height, 0, GL_BGRA, GL_UNSIGNED_BYTE, sprites[i].data);
     }
}

void OpenGL::_GenerateFont ( void )
{
     // Pre-defined values, eh?
     int imgSizeX = 512;
     int imgSizeY = 128;
     int chSpaceX = 16;
     int chSpaceY = 16;
     int startChar = 32;
     int num_x;

     fontNewLineSpace = (float)chSpaceY;
     num_x = imgSizeX/chSpaceX;

     float x, y, x2, y2;
     y2 = (float)(chSpaceY/(float)imgSizeY);
     fontListHolder = glGenLists(256);
     for(int i = startChar; i < 256; i++)
     {
          x = (((i-startChar) % num_x) * chSpaceX) / (float)imgSizeX;
          y = 1.0f - ((((i-startChar) / num_x) * chSpaceY) / (float)imgSizeY);
          x2 = g_font->m_fontWidth[i]/(float)imgSizeX;

          glNewList(fontListHolder+i, GL_COMPILE);
          {
               glBegin(GL_QUADS);
                    glTexCoord2f( x, y );             glVertex2i(0, 0);
                    glTexCoord2f( x + x2, y );        glVertex2i((int)g_font->m_fontWidth[i], 0);
                    glTexCoord2f( x + x2, y - y2 );   glVertex2i((int)g_font->m_fontWidth[i], chSpaceY);
                    glTexCoord2f( x, y - y2 );        glVertex2i(0, chSpaceY);
               glEnd();
               glTranslatef((float)g_font->m_fontWidth[i], 0.0f, 0.0f);
          }
          glEndList();
     }
}

void OpenGL::RenderText ( Vector position, char* text, ECOLOR color )
{
    if (!text)
    {
        return;
    }
    const unsigned lenght = strlen(text);

    char *context = NULL;
    char *p = strtok_s (text,"\n", &context);

    glPushAttrib ((GL_CURRENT_BIT | GL_TEXTURE_BIT));
    glBindTexture( GL_TEXTURE_2D, textures[FONT_TEXTURE_ID] );
    glColor4f ( color.r , color.g, color.b, color.a );
    short cur_y = 0;
    while ( p )
    {
        glPushMatrix();
        glTranslatef( (float)position.x, ((float)position.y+cur_y*fontNewLineSpace), 0.0f );
        glListBase( fontListHolder );
        glCallLists ( strlen(p), GL_UNSIGNED_BYTE, p );
        glPopMatrix();
        cur_y++;
        p = strtok_s(NULL,"\n", &context);
    }

	glPopAttrib();

}

void OpenGL::RenderRectangle (Vector2 position, ECOLOR color, int texture_id, Vector2 texture_position)
{
    bool alpha = color != ECOLOR();

    if (alpha)
    {
        glEnable(GL_BLEND);
        glColor4f(color.r,color.g,color.b,color.a);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    if ( texture_id != 0 )
    {
        glBindTexture( GL_TEXTURE_2D, textures[texture_id-1] );

        glBegin(GL_QUADS);
            glTexCoord2f( texture_position.start_x, texture_position.start_y );   glVertex2f( position.start_x, position.end_y );
            glTexCoord2f( texture_position.end_x, texture_position.start_y );     glVertex2f( position.end_x, position.end_y );
            glTexCoord2f( texture_position.end_x, texture_position.end_y );       glVertex2f( position.end_x, position.start_y );
            glTexCoord2f( texture_position.start_x, texture_position.end_y );     glVertex2f( position.start_x, position.start_y );
        glEnd();

        glBindTexture( GL_TEXTURE_2D, 0 );
    }
    else
    {
        glBegin(GL_QUADS);
            glVertex2f( position.start_x, position.end_y );
            glVertex2f( position.end_x, position.end_y );
            glVertex2f( position.end_x, position.start_y );
            glVertex2f( position.start_x, position.start_y );
        glEnd();
    }

    if (alpha)
    {
        glDisable(GL_BLEND);
        glColor4f(1.0,1.0,1.0,1.0);
    }
}

void OpenGL::RenderList (GUI_RectangleList *list, ECOLOR color, Vector pos)
{
    Vector2* object = list->GetDimensions();
    Vector2* texture = list->GetTexturePosition();
    bool alpha = color != ECOLOR();

    glBindTexture( GL_TEXTURE_2D, textures[list->GetTexture()-1] );

    if (alpha)
    {
        glEnable(GL_BLEND);
        glColor4f(color.r,color.g,color.b,color.a);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glBegin(GL_QUADS);
        for (int i = 0; i < list->GetSize(); i++)
        {
            glTexCoord2f((texture+i)->start_x, (texture+i)->start_y);
            glVertex2f((object+i)->start_x+pos.x, (object+i)->end_y+pos.y);
            
            glTexCoord2f((texture+i)->end_x, (texture+i)->start_y);
            glVertex2f((object+i)->end_x+pos.x, (object+i)->end_y+pos.y);
            
            glTexCoord2f((texture+i)->end_x, (texture+i)->end_y);
            glVertex2f((object+i)->end_x+pos.x, (object+i)->start_y+pos.y);
            
            glTexCoord2f((texture+i)->start_x, (texture+i)->end_y);
            glVertex2f((object+i)->start_x+pos.x, (object+i)->start_y+pos.y);
        }
    glEnd();
    if (alpha)
    {
        glDisable(GL_BLEND);
        glColor4f(1.0,1.0,1.0,1.0);
    }
    glBindTexture( GL_TEXTURE_2D, 0 );
}

void OpenGL::SetRenderArea (Vector2 *area)
{
    if (area)
    {
        glScissor(area->start_x, m_height-area->start_y, area->end_x, area->end_y);
        glEnable(GL_SCISSOR_TEST);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }
}