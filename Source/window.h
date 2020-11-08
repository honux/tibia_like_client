#pragma once

#include "definitions.h"
#include "screen.h"
#include "vector.h"
#include "sprloader.h"
#include "ecolor.h"
#include "GUI_rectanglelist.h"

class Window
{
protected:
    bool m_active;
    int m_width, m_height, bits;
    GLuint textures[SPRITES_NUMBER];

    HDC       hDC;
    HGLRC     hRC;
    HWND      hWnd;
    HINSTANCE hInstance;

    Window()
    {
        hDC       = NULL;		// Private GDI Device Context
        hRC       = NULL;		// Permanent Rendering Context
        hWnd      = NULL;       // Holds Our Window Handle

        sprites   = NULL;

        m_width   = 0;
        m_height  = 0;

        /* Keyboard and Mouse data */
        m_active = true;
        for ( int i = 0; i < SPRITES_NUMBER; i++ )
        {
            textures[i] = i+1;
        }
    };

    public:
    Sprite *sprites;

    // Create an initial screen
    virtual bool Create ( char *_name, int _width, int _height, int _bits) = 0;
    // Destroy the screen :(
    virtual void Destroy ( void ) = 0;
    // Resize the current screen
    virtual void Resize ( int _width, int _height ) = 0;
    // Refresh the current screen
    virtual void DrawScene ( void ) = 0;
    // Display the current screen
    virtual void Display ( void ) = 0;

    /* Rendering stuff, distributed in 3 dif functions ( Maybe more later? ) */
    virtual void RenderRectangle (Vector2 position, ECOLOR color, int texture_id = 0, Vector2 texture_position = Vector2(0.0, 0.0, 1.0, 1.0)) = 0;
    virtual void RenderText (Vector position, char* text, ECOLOR color = ECOLOR()) = 0;
    virtual void RenderList (GUI_RectangleList *list, ECOLOR color, Vector pos = Vector()) = 0;
    virtual void SetRenderArea (Vector2 *area) = 0;

    // Change the window active status
    inline void SetActive (bool _active);

    inline Vector GetDimensions (void);

    /* Return types */
    inline bool IsActive (void);

    friend LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    friend class LoginScreen;
};

inline void Window::SetActive (bool _active)
{
    m_active = _active;
}

inline Vector Window::GetDimensions (void)
{
    return Vector(m_width, m_height);
}

inline bool Window::IsActive (void)
{ 
    return m_active;
}

