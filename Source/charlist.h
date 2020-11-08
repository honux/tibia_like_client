#pragma once

#include <list>
#include "GUI_element.h"
#include "GUI_box.h"
#include "GUI_scrollbar.h"

struct Character
{
    char *name;
    unsigned long id;
    
    char *world;
    char *ip;
    short port;
};

class CharacterList: public GUI_Element
{
public:
    CharacterList (void);
    CharacterList (int width, int height, Vector position);
    ~CharacterList (void);

    void AddCharacter (unsigned long id, char *name, char *world, char *ip, short port);

    void Render (Vector pos = Vector(), ECOLOR *style = NULL);
    void MouseEvent (MouseE *e);
    void KeyPress (KeyE *e);

    inline void Push (GUI_Element *thing);
    inline void Remove (GUI_Element *thing);

private:
    bool m_isDragging;

    std::list<Character*> m_charList;
    std::list <GUI_Element*> m_content;

    GUI_RectangleList* m_design;
    GUI_ScrollBar *m_scroll;
    Character *m_selected;

    GUI_Text m_title;

    MouseE m_lastMouse;
};

inline void CharacterList::Push (GUI_Element *thing)
{
    m_content.push_back(thing);
    thing->SetParent(this);
}

inline void CharacterList::Remove (GUI_Element *thing)
{
    m_content.remove(thing);
    delete thing;
}