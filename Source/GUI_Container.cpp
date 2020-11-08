#include "GUI_container.h"
#include "window.h"
#include "font.h"

extern Window *g_window;
extern Font *g_font;

void ContainerSlot::Render (Vector pos, char *number)
{
    Vector2 position(pos.x, pos.y, pos.x+36, pos.y+36);
    g_window->RenderRectangle(position, ECOLOR(), 3, Vector2(131.0/256.0, 60.0/128.0, 167.0/256.0, 96.0/128.0));
    g_window->RenderText(Vector(pos.x+18-g_font->GetSize(number)/2, pos.y+16), number);
    /*
    if (id != 0)
    {
        Vector2 item(pos.x, pos.y, pos.x+32, pos.y+32);
        g_window->RenderRectangle(item, ECOLOR(), id);
    }
    */
}

GUI_Container::GUI_Container (void)
: GUI_Element(),  m_slots(NULL), m_size(0), m_flags(0), m_scroll(NULL), m_isDragging(false), m_selected(NULL)
{
    m_flags = 0;
    m_design = new GUI_RectangleList(13, 4);
}

GUI_Container::GUI_Container (int width, int height, Vector position, int size)
: GUI_Element(width, height, position), m_size(size), m_isDragging(false), m_selected(NULL)
{
    m_flags = 0;
    m_design = new GUI_RectangleList(13, 4);
    m_slots = new ContainerSlot[size];

    int correction = (size%3==0)?0:1;
    m_scroll = new GUI_ScrollBar(18, height-20, Vector(width-18, 20), ((size/3)+correction)*38, this);

    m_content.push_back(m_scroll);

    _UpdatePosition();
}

GUI_Container::~GUI_Container (void)
{
    delete[] m_slots;
    delete m_design;
    m_buttonsList.clear();
    m_content.clear();
}

void GUI_Container::Render (Vector pos, ECOLOR *style)
{
    g_window->RenderList(m_design, m_color, m_position);
    int textCorrection = 0;
    for (std::list<GUI_Button*>::iterator it = m_buttonsList.begin(); it != m_buttonsList.end(); it++)
    {
        (*it)->Render(m_position, &m_color);
        textCorrection -= 8;
    }
    g_window->RenderText(Vector(m_position.x+textCorrection+m_width/2-g_font->GetSize("Container")/2, m_position.y+5), "Container");

    if (!HasFlag(FLAG_ISMINIMIZED))
    {
        Vector window = g_window->GetDimensions();
        Vector2 container(m_position.x+6, m_position.y+m_height-3, m_width-12, m_height-24);
        g_window->SetRenderArea(&container);
        char name[50];
        int scroll = m_scroll->GetScrollBarPosition();
        for (int i = 0; i < m_size; i++)
        {
            sprintf(name, "%d", i+1);
            Vector position( m_position.x+6+(i%3)*38, m_position.y+21+(i/3)*38-scroll );
            m_slots[i].Render(position, name);
        }
        g_window->SetRenderArea(NULL);
        //g_window->RenderRectangle(Vector2(m_position.x+6, m_position.y+23, m_position.x+m_width-18, m_position.y+m_height-4), ECOLOR(0.0, 1.0, 1.0, 1.0));
        m_scroll->Render(m_position);
    }
}

void GUI_Container::MouseEvent (MouseE *e)
{
    if (e->State == EMOUSE_BUTTON_DOWN)
    {
        if (e->X >= m_position.x+4 && e->X <= m_position.x-4+m_width && e->Y >= m_position.y+4 && e->Y <= m_position.y+20)
        {
            GUI_Button *selected = NULL;
            Vector elepos;
            for (std::list<GUI_Button*>::iterator it = m_buttonsList.begin(); it != m_buttonsList.end(); it++)
            {
                elepos = (*it)->GetPosition() + m_position;
                if (e->X >= elepos.x && e->Y >= elepos.y && e->X <= elepos.x+(*it)->GetWidth() && e->Y <= elepos.y+(*it)->GetHeight())
                {
                    selected = (*it);
                }
            }
            if (selected)
            {
                selected->MouseEvent(e);
            }
            else
            {
                m_isDragging = true;
                m_lastMouse = *e;
            }
        }
        else
        {
            GUI_Element *selected = NULL;
            Vector elepos;

            for (std::list<GUI_Element*>::iterator it = m_content.begin(); it != m_content.end(); it++)
            {
                elepos = (*it)->GetPosition() + m_position;
                if (e->X >= elepos.x && e->Y >= elepos.y && e->X <= elepos.x+(*it)->GetWidth() && e->Y <= elepos.y+(*it)->GetHeight())
                {
                    selected = (*it);
                }
            }
            if (selected)
            {
                m_selected = selected;
                selected->MouseEvent(e);
            }
        }
    }
    else if (e->State == EMOUSE_MOVE)
    {
        if (m_isDragging)
        {
            m_position.x = m_position.x + e->X - m_lastMouse.X;
            m_position.y = m_position.y + e->Y - m_lastMouse.Y;
            m_lastMouse = *e;
        }
        else if (m_selected)
        {
            m_selected->MouseEvent(e);
        }
    }
    else if (e->State == EMOUSE_BUTTON_UP)
    {
        if (e->X >= m_position.x+4 && e->X <= m_position.x-4+m_width && e->Y >= m_position.y+4 && e->Y <= m_position.y+20)
        {
            m_selected = NULL;

            GUI_Button *selected = NULL;
            Vector elepos;
            for (std::list<GUI_Button*>::iterator it = m_buttonsList.begin(); it != m_buttonsList.end(); it++)
            {
                elepos = (*it)->GetPosition() + m_position;
                if (e->X >= elepos.x && e->Y >= elepos.y && e->X <= elepos.x+(*it)->GetWidth() && e->Y <= elepos.y+(*it)->GetHeight())
                {
                    selected = (*it);
                }
            }
            if (selected)
            {
                selected->MouseEvent(e);
            }
            else
            {
                m_isDragging = false;
            }
        }
        else
        {
            GUI_Element *selected = NULL;
            Vector elepos;
            for (std::list<GUI_Element*>::iterator it = m_content.begin(); it != m_content.end(); it++)
            {
                elepos = (*it)->GetPosition() + m_position;
                if (e->X >= elepos.x && e->Y >= elepos.y && 
                    e->X <= elepos.x+(*it)->GetWidth() && e->Y <= elepos.y+(*it)->GetHeight())
                {
                    selected = (*it);
                }
            }
            if (selected)
            {
                selected->MouseEvent(e);
            }
        }
    }

}

void GUI_Container::KeyPress (KeyE *e)
{
}

/* Private Functions */
void GUI_Container::_UpdatePosition (void)
{
    m_design->Add(Vector2(0, 0, 4, 3),
        Vector2(0.0/256.0, 124.0/128.0, 4./256.0, 127.0/128.0), 0);
    m_design->Add(Vector2(4, 0, m_width-3, 3),
        Vector2(4.0/256.0, 124.0/128.0, 13./256.0, 127.0/128.0), 1);
    m_design->Add(Vector2(m_width-3, 0, m_width, 3),
        Vector2(13.0/256.0, 124.0/128.0, 16./256.0, 127.0/128.0), 2);


    if (HasFlag(FLAG_ISMINIMIZED))
    {
        m_design->Add(Vector2(0, 3, 6, 18),
            Vector2(0.0/256.0, 109.0/128.0, 6./256.0, 124.0/128.0), 3);
        m_design->Add(Vector2(6, 3, m_width-4, 18),
            Vector2(6.0/256.0, 109.0/128.0, 12./256.0, 124.0/128.0), 4);
        m_design->Add(Vector2(m_width-4, 3, m_width, 18),
            Vector2(12.0/256.0, 109.0/128.0, 16./256.0, 124.0/128.0), 5);

        m_design->Add(Vector2(0, 18, 4, 21),
            Vector2(0.0/256.0, 105.0/128.0, 4./256.0, 108.0/128.0), 6);
        m_design->Add(Vector2(4, 18, m_width-3, 21),
            Vector2(4.0/256.0, 105.0/128.0, 13./256.0, 108.0/128.0), 7);
        m_design->Add(Vector2(m_width-3, 18, m_width, 21),
            Vector2(13.0/256.0, 105.0/128.0, 16./256.0, 108.0/128.0), 8);
    }
    else
    {
        m_design->Add(Vector2(0, 3, 6, m_height-3),
            Vector2(0.0/256.0, 109.0/128.0, 6./256.0, 124.0/128.0), 3);
        m_design->Add(Vector2(6, 3, m_width-4, m_height-3),
            Vector2(6.0/256.0, 109.0/128.0, 12./256.0, 124.0/128.0), 4);
        m_design->Add(Vector2(m_width-4, 3, m_width, m_height-3),
            Vector2(12.0/256.0, 109.0/128.0, 16./256.0, 124.0/128.0), 5);

        m_design->Add(Vector2(0, m_height-3, 4, m_height),
            Vector2(0.0/256.0, 105.0/128.0, 4./256.0, 108.0/128.0), 6);
        m_design->Add(Vector2(4, m_height-3, m_width-3, m_height),
            Vector2(4.0/256.0, 105.0/128.0, 13./256.0, 108.0/128.0), 7);
        m_design->Add(Vector2(m_width-3, m_height-3, m_width, m_height),
            Vector2(13.0/256.0, 105.0/128.0, 16./256.0, 108.0/128.0), 8);
    }
}

void GUI_Container::ChangeMinimized (GUI_Button *button)
{
    if (HasFlag(FLAG_ISMINIMIZED))
    {
        RemoveFlag(FLAG_ISMINIMIZED);
        button->SetStyle(BUTTON_MINUS);
    }
    else
    {
        SetFlag(FLAG_ISMINIMIZED);
        button->SetStyle(BUTTON_PLUS);
    }
    _UpdatePosition();
}