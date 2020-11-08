#pragma once

#include "GUI_element.h"
#include "GUI_button.h"
#include "vector.h"

class GUI_ScrollBar: public GUI_Element
{
public:
    GUI_ScrollBar (void);
    GUI_ScrollBar (int width, int height, Vector position, int maxSize, GUI_Element *parent);
    ~GUI_ScrollBar (void);

    void Render (Vector pos = Vector(), ECOLOR *style = NULL);
    void MouseEvent (MouseE *e);
    void KeyPress (KeyE *e);
    int GetScrollBarPosition (void);

private:
    void _MoveScrollUp (void);
    void _MoveScrollDown (void);

private:
    int m_maxSize;
    int m_displayStart;
    int m_displayEnd;

    bool m_isDragging;

    GUI_Button *m_arrowUp;
    GUI_Button *m_arrowDown;
    GUI_Button *m_scroll;

    MouseE m_lastMouse;
};