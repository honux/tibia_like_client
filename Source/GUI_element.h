#pragma once

#include <windows.h>
#include "vector.h"
#include "ecolor.h"
#include "screen.h"

extern Screen *g_screen;

struct KeyE;
struct MouseE;

class GUI_Element
{
    friend class LoginScreen;
public:

    GUI_Element (void)
    : m_width(0), m_height(0), m_position(), m_textId(0), m_textPos(), m_moveable(false), m_selectable(false), m_parent(NULL)
    {
    };

    GUI_Element (int width, int height, Vector position, int texture = 0, Vector2 texPos = Vector2())
    : m_width(width), m_height(height), m_position(position), m_textId(texture), m_textPos(texPos), m_moveable(false), m_selectable(false), m_parent(NULL)
    {
    };

    ~GUI_Element (void)
    {
    };

    virtual void Render (Vector pos = Vector(), ECOLOR *style = NULL) = 0 ;
    virtual void MouseEvent (MouseE *e) = 0;
    virtual void KeyPress (KeyE *e) = 0;

    /* Set new var values */
    inline void SetPosition (Vector _position);
    inline void SetTextureId (int _texture);
    inline void SetTexturePosition (Vector2 _position);
    inline void SetSize (int _width, int _height);
    inline void SetWidth (int _width);
    inline void SetHeight (int _height);
    inline void SetMoveable (bool _moveable);
    inline void SetSelectable (bool _selectable);
    inline void SetColor (ECOLOR color);
    inline void SetParent (GUI_Element *element);
    /* Just return the var values */
    inline Vector GetPosition (void);
    inline int GetTextureId (void);
    inline Vector2 GetTexturePosition (void);
    inline Vector GetSize (void);
    inline int GetWidth (void);
    inline int GetHeight (void);
    inline bool IsMoveable (void);
    inline bool IsSelectable (void);
    inline GUI_Element* GetParent (void);

protected:
    int m_width;
    int m_height;
    int m_textId;
    bool m_moveable;
    bool m_selectable;

    Vector m_position;
    Vector2 m_textPos;
    ECOLOR m_color;
    GUI_Element *m_parent;
};

inline void GUI_Element::SetPosition (Vector _position)
{
    m_position = _position; 
}

inline void GUI_Element::SetTextureId (int _texture)
{
    m_textId = _texture;
}

inline void GUI_Element::SetTexturePosition (Vector2 _position)
{
    m_textPos = _position;
}

inline void GUI_Element::SetSize (int _width, int _height)
{ 
    m_width = _width;
    m_height = _height;
}

inline void GUI_Element::SetWidth (int _width)
{ 
    m_width = _width;
}

inline void GUI_Element::SetHeight (int _height)
{ 
    m_height = _height;
}

inline void GUI_Element::SetMoveable (bool _moveable)
{ 
    m_moveable = _moveable; 
}

inline void GUI_Element::SetSelectable (bool _selectable)
{ 
    m_selectable = _selectable; 
}

inline void GUI_Element::SetColor (ECOLOR color)
{ 
    m_color = color; 
}

inline void GUI_Element::SetParent (GUI_Element *element)
{
    m_parent = element;
}

inline Vector GUI_Element::GetPosition (void)
{
    return m_position;
}

inline int GUI_Element::GetTextureId (void)
{
    return m_textId;
}

inline Vector2 GUI_Element::GetTexturePosition (void)
{
    return m_textPos;
}

inline Vector GUI_Element::GetSize (void)
{
    return Vector(m_width, m_height);
}

inline int GUI_Element::GetWidth (void)
{
    return m_width;
}

inline int GUI_Element::GetHeight (void)
{
    return m_height;
}

inline bool GUI_Element::IsMoveable (void)
{
    return m_moveable;
}

inline bool GUI_Element::IsSelectable (void)
{
    return m_selectable;
}

inline GUI_Element* GUI_Element::GetParent (void)
{
    return m_parent;
}