#pragma once

#include "GUI_element.h"
#include "GUI_rectanglelist.h"
#include "events.h"
#include "definitions.h"

class GUI_TextBox: public GUI_Element
{
public:
    GUI_TextBox (void);
    GUI_TextBox (int width, int height, Vector position, int maxsize, int flags = 0);
    ~GUI_TextBox (void);

    void Render (Vector pos = Vector(), ECOLOR *style = NULL);
    void MouseEvent (MouseE *e);
    void KeyPress (KeyE *e);
    
    inline void RenderCursor (bool status);
    inline bool HasFlag (FLAGS flag);
    inline void SetFlag (FLAGS flag);
    inline char* const GetText (void);

private:
    void _UpdatePosition (void);
    void _HandleAccentuation (unsigned char key);
    bool _AddChar (char letter);
    bool _RemoveLeftChar (void);
    bool _RemoveRightChar (void);
    void _UpdateText (void);

private:
    int m_strMaxSize;   // String max size 
    int m_cursorPos;    // Position of the cursor
    int m_curLenght;    // Current lenght of the string
    int m_flags;
    
    char *m_string;     // complete string holder

    char *m_display;
    int m_anchor;
    int m_drawEnd;

    bool m_renderCursor;
    GUI_RectangleList* m_content;
};

inline void GUI_TextBox::RenderCursor (bool status)
{
    m_renderCursor = status;
}

inline bool GUI_TextBox::HasFlag (FLAGS flag)
{
    return (m_flags&flag);
}

inline void GUI_TextBox::SetFlag (FLAGS flag)
{
    m_flags = (m_flags|flag);
}

inline char* const GUI_TextBox::GetText (void)
{
    return m_string;
}
