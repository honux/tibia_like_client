#pragma once

#include "GUI_element.h"
#include "GUI_rectanglelist.h"
#include "GUI_scrollbar.h"
#include <list>
#include "definitions.h"

struct ContainerSlot
{
    // Maybe replaced by Items later?
    int id;
    void Render (Vector pos, char *number);
};

class GUI_Container: public GUI_Element
{
public:
    GUI_Container (void);
    GUI_Container (int width, int height, Vector position, int size);
    ~GUI_Container (void);

    void Render (Vector pos = Vector(), ECOLOR *style = NULL);
    void MouseEvent (MouseE *e);
    void KeyPress (KeyE *e);

    void ChangeMinimized (GUI_Button *button);

    inline bool HasFlag (int flag);
    inline void SetFlag (int flag);
    inline void RemoveFlag (int flag);
    inline void Push (GUI_Element *thing);
    inline void PushButton (GUI_Button *thing);
    inline void Remove (GUI_Element *thing);

private:
    void _UpdatePosition (void);

private:
    int m_size;
    int m_flags;
    bool m_isDragging;
    bool m_temp;

    ContainerSlot *m_slots;
    GUI_RectangleList* m_design;
    std::list <GUI_Element*> m_content;
    std::list <GUI_Button*> m_buttonsList;
    GUI_ScrollBar *m_scroll;
    GUI_Element *m_selected;

    MouseE m_lastMouse;
};

inline bool GUI_Container::HasFlag (int flag)
{
    return (m_flags&flag);
}

inline void GUI_Container::SetFlag (int flag)
{
    m_flags = (m_flags|flag);
}

inline void GUI_Container::RemoveFlag (int flag)
{
    m_flags = (m_flags^flag);
}

inline void GUI_Container::Push (GUI_Element *thing)
{
    m_content.push_back(thing);
    thing->SetParent(this);
}

inline void GUI_Container::PushButton (GUI_Button *thing)
{
    m_buttonsList.push_back(thing);
    thing->SetParent(this);
}

inline void GUI_Container::Remove (GUI_Element *thing)
{
    m_content.remove(thing);
    delete thing;
}