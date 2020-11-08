#pragma once

#include "GUI_element.h"
#include "window.h"

extern Window *g_window;

class GUI_Rectangle: public GUI_Element
{
    public:
    GUI_Rectangle (void)
    : GUI_Element()
    {
    };

    GUI_Rectangle (int width, int height, Vector position, int texture, Vector2 texPos)
    : GUI_Element(width, height, position, texture, texPos)
    {
    };

    ~GUI_Rectangle (void)
    {
    };

    inline void Render (Vector pos = Vector(), ECOLOR *style = NULL);
    inline void MouseEvent (MouseE *e);
    inline void KeyPress (KeyE *e);
};

inline void GUI_Rectangle::Render (Vector pos, ECOLOR *style)
{
    Vector final = pos + m_position;
    Vector2 position(final.x, final.y, final.x+m_width, final.y+m_height);
    if (style)
    {
        g_window->RenderRectangle(position, *style, m_textId, m_textPos);
    }
    else
    {
        g_window->RenderRectangle(position, m_color, m_textId, m_textPos);
    }
}

inline void GUI_Rectangle::MouseEvent (MouseE *e)
{
}

inline void GUI_Rectangle::KeyPress (KeyE *e)
{
}
