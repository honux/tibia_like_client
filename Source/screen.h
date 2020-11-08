#pragma once

#include <list>
#include "events.h"

class GUI_Element;
class GUI_TextBox;
class GUI_Box;


class Screen
{
    friend class OpenGL;
public:
    virtual void KeyEvent (KeyE *e) = 0;
    virtual void MouseEvent (MouseE *e) = 0;

    virtual void Resize (int width, int height) = 0;
    virtual void RemoveElement (GUI_Element* thing) = 0;

    inline MouseE GetLastMouse (void);

protected:
    std::list <GUI_Element*> m_screenElement;
    GUI_Element *m_selected;
    GUI_Element *m_dragging;
    GUI_Box *m_locked;

    GUI_TextBox *m_curFocus;

    bool m_displayFps;
};