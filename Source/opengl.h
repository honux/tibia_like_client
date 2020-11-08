#pragma once

#include <windows.h>
#include "window.h"
#include "vector.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

class OpenGL : public Window
{
    int fontListHolder;
    float fontNewLineSpace;

    // Generate Textures?
    void _GenerateTextures ( void );
    // ...
    void _GenerateFont ( void );
public:
    OpenGL ( void );

    bool Create ( char *_name, int _width, int _height, int _bits);
    void Destroy ( void );
    void Resize ( int _width, int _height );
    void DrawScene ( void );
    void Display ( void );

    void RenderText (Vector position, char* text, ECOLOR color);
    void RenderRectangle (Vector2 position, ECOLOR color, int texture_id, Vector2 texture_position);
    void RenderList (GUI_RectangleList *list, ECOLOR color, Vector pos);
    void SetRenderArea (Vector2 *area);
};