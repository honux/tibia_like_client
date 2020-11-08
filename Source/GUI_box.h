#pragma once

#include "GUI_element.h"
#include "GUI_rectanglelist.h"
#include "GUI_text.h"
#include <list>

class GUI_Box: public GUI_Element
{
public:
    GUI_Box (void);
    GUI_Box (int width, int height, Vector position, char *title);
    ~GUI_Box (void);

    void Render (Vector pos = Vector(), ECOLOR *style = NULL);
    void MouseEvent (MouseE *e);
    void KeyPress (KeyE *e);

    inline void Push (GUI_Element *thing);
    inline void Remove (GUI_Element *thing);
private:
    void _UpdatePosition (void);
private:
    char *m_titleStr;
    bool m_isDragging;

    std::list <GUI_Element*> m_content;
    GUI_RectangleList* m_design;
    GUI_Text m_title;

    MouseE m_lastMouse;
};

inline void GUI_Box::Push (GUI_Element *thing)
{
    m_content.push_back(thing);
    thing->SetParent(this);
}

inline void GUI_Box::Remove (GUI_Element *thing)
{
    m_content.remove(thing);
    delete thing;
}