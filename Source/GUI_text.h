#pragma once

#include "GUI_element.h"
#include "window.h"

extern Window    *g_window;

class GUI_Text: public GUI_Element
{
    char* m_string;


public:
    GUI_Text (void)
    : m_string(NULL)
    {
        m_position = Vector();
    }
    GUI_Text (Vector position, char* name, ECOLOR color = ECOLOR())
    {
        m_color = color;
        m_position = position;
        if (name)
        {
            m_string = new char[strlen(name)+1];
            strcpy(m_string, name);
        }
        else
        {
            m_string = NULL;
        }

    }
    ~GUI_Text (void)
    {
       delete[] m_string;
    }

    inline void Render (Vector pos = Vector(), ECOLOR *style = NULL);
    inline void MouseEvent (MouseE *e);
    inline void KeyPress (KeyE *e);

    inline void SetString (char* name);
};

inline void GUI_Text::Render (Vector pos, ECOLOR *style)
{ 
    ECOLOR color = (style)?*style:m_color;
    g_window->RenderText(pos+m_position, m_string, color);
}

inline void GUI_Text::MouseEvent (MouseE *e)
{
}

inline void GUI_Text::KeyPress (KeyE *e)
{
}

inline void GUI_Text::SetString (char* name)
{
    delete[] m_string;

    m_string = new char[strlen(name)+1];
    strcpy( m_string, name );
}