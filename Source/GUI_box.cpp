#include "GUI_box.h"
#include "font.h"
#include "window.h"

#include "events.h"

extern Window *g_window;
extern Font *g_font;

GUI_Box::GUI_Box (void)
: GUI_Element(), m_isDragging(false)
{
    m_design = new GUI_RectangleList(13, 3);
}

GUI_Box::GUI_Box (int width, int height, Vector position, char *title)
: GUI_Element(width, height, position), m_isDragging(false)
{
    m_design = new GUI_RectangleList(13, 3);

    m_titleStr = new char[strlen(title)+1];
    strcpy(m_titleStr, title);

    _UpdatePosition();
}

GUI_Box::~GUI_Box (void)
{
    for (std::list<GUI_Element*>::iterator it = m_content.begin(); it != m_content.end(); it++)
    {
        delete (*it);
    }
    delete[] m_titleStr;
    delete m_design;
}

void GUI_Box::_UpdatePosition (void)
{
    // Borders
    m_design->Add(Vector2(0, 0, 11, 11),
        Vector2(105.0/256.0, 85.0/128.0, 116.0/256.0, 96.0/128.0), 0);
        
    m_design->Add(Vector2(12, 0, m_width-12, 3),
        Vector2(13.0/256.0, 124.0/128.0, 242.0/256.0, 127.0/128.0), 1);

    m_design->Add(Vector2(m_width-11, 0, m_width, 11),
        Vector2(118.0/256.0, 85.0/128.0, 129.0/256.0, 96.0/128.0), 2);
        
    m_design->Add(Vector2(m_width-3, 12, m_width, m_height-12),
        Vector2(1.0/256.0, 13.0/128.0, 4.0/256.0, 115.0/128.0), 3);
    
    m_design->Add(Vector2(m_width-11, m_height-11, m_width, m_height),
        Vector2(118.0/256.0, 72.0/128.0, 129.0/256.0, 83.0/128.0), 4);
        
    m_design->Add(Vector2(12, m_height-3, m_width-12, m_height),
        Vector2(13.0/256.0, 124.0/128.0, 242.0/256.0, 127.0/128.0), 5);

    m_design->Add(Vector2(0, m_height-11, 11, m_height),
        Vector2(105.0/256.0, 72.0/128.0, 116.0/256.0, 83.0/128.0), 6);

    m_design->Add(Vector2(0, 12, 3, m_height-12),
        Vector2(1.0/256.0, 13.0/128.0, 4.0/256.0, 115.0/128.0), 7);

    m_design->Add(Vector2(4, 25, 6, 0+m_height-26),
        Vector2(5.0/256.0, 27.0/128.0, 7.0/256.0, 101.0/128.0), 8);
        
    m_design->Add(Vector2(m_width-6, 25, m_width-4, m_height-26),
        Vector2(249.0/256.0, 27.0/128.0, 251.0/256.0, 101.0/128.0), 9);
        
    // Title
    m_design->Add(Vector2(4, 4, m_width-4, 24),
        Vector2(5.0/256.0, 103.0/128.0, 251./256.0, 123.0/128.0), 10);

    // Message Area
    m_design->Add(Vector2(6, 25, m_width-6, m_height-26),
        Vector2(8.0/256.0, 64.0/128.0, 39.0/256.0, 96.0/128.0), 11);
        
    // Botton
    m_design->Add(Vector2(4, m_height-25, m_width-4, m_height-4),
        Vector2(5.0/256.0, 6.0/128.0, 250./256.0, 27.0/128.0), 12);

    m_title.SetString(m_titleStr);
    m_title.SetPosition(Vector((172*(m_width-3)/246)-g_font->GetSize(m_titleStr)/2, 0+9));
}

void GUI_Box::Render (Vector pos, ECOLOR *style)
{
    g_window->RenderList(m_design, m_color, m_position);
    m_title.Render(m_position);
    for (std::list<GUI_Element*>::iterator it = m_content.begin(); it != m_content.end(); it++)
    {
        (*it)->Render(m_position, &m_color);
    }
}

void GUI_Box::MouseEvent (MouseE *e)
{
    if (e->State == EMOUSE_BUTTON_DOWN)
    {
        if (e->X >= m_position.x+4 && e->X <= m_position.x-4+m_width && e->Y >= m_position.y+4 && e->Y <= m_position.y+20)
        {
            m_isDragging = true;
            m_lastMouse = *e;
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
    }
    else if (e->State == EMOUSE_BUTTON_UP)
    {
        if (e->X >= m_position.x+4 && e->X <= m_position.x-4+m_width && e->Y >= m_position.y+4 && e->Y <= m_position.y+20)
        {
            m_isDragging = false;
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

void GUI_Box::KeyPress (KeyE *e)
{
}

/*
bool GUI_Box::CanDrag (MouseE *e)
{
    if (e->X >= m_position.x+4 && e->X <= m_position.x-4+m_width
        && e->Y >= m_position.y+4 && e->Y <= m_position.y+20)
    {
        return true;
    }
    return false;
}
*/