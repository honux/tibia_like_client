#include "GUI_scrollbar.h"

GUI_ScrollBar::GUI_ScrollBar (void)
: GUI_Element(), m_scroll(NULL), m_arrowUp(NULL), m_arrowDown(NULL), m_isDragging(false)
{
}

GUI_ScrollBar::GUI_ScrollBar (int width, int height, Vector position, int maxSize, GUI_Element *parent)
: GUI_Element(width, height, position), m_isDragging(false), m_maxSize(maxSize)
{
    m_parent = parent;
    m_arrowUp = new GUI_Button(16, 14, Vector(0, 0), NULL, 4);
    m_arrowDown = new GUI_Button(16, 14, Vector(0, height-14), NULL, 5);
    if (height >= maxSize)
    {
        m_scroll = NULL;
    }
    else
    {
        int scrollS = (height-28)*((double)height/maxSize);
        m_scroll = new GUI_Button(16, scrollS, Vector(0, 14), NULL, 6);
    }
}

GUI_ScrollBar::~GUI_ScrollBar (void)
{
    delete m_arrowUp;
    delete m_arrowDown;
    delete m_scroll;
}

void GUI_ScrollBar::Render (Vector pos, ECOLOR *style)
{
    m_arrowUp->Render(pos+m_position);
    m_arrowDown->Render(pos+m_position);

    if (m_scroll)
    {
        m_scroll->Render(pos+m_position);
    }
}

void GUI_ScrollBar::MouseEvent (MouseE *e)
{
    Vector parent = ((m_parent)?m_parent->GetPosition():Vector()) + m_position;
    if (e->State == EMOUSE_BUTTON_DOWN)
    {
        if (m_scroll)
        {
            Vector scroll = m_scroll->GetPosition();
            if (e->X >= scroll.x + parent.x && e->X <= parent.x + scroll.x + m_scroll->GetWidth() && e->Y >= scroll.y + parent.y && e->Y <= parent.y + scroll.y + m_scroll->GetHeight())
            {
                m_isDragging = true;
                m_lastMouse = *e;
            }
        }
    }
    else if (e->State == EMOUSE_MOVE)
    {
        if (m_isDragging && m_scroll)
        {
            if (e->Y > parent.y + 14 && e->Y < parent.y + m_height +14)
            {
                Vector scroll = m_scroll->GetPosition();
                Vector position(0, e->Y-m_lastMouse.Y);

                int height = m_scroll->GetHeight();
                if (scroll.y + position.y > 13 && scroll.y + position.y + height < m_height - 13)
                {
                    m_scroll->SetPosition(m_scroll->GetPosition()+position);
                    m_scroll->SetStyle(BUTTON_SCROLL);
                }
                m_lastMouse = *e;
            }
        }
    }
    else if (e->State == EMOUSE_BUTTON_UP)
    {
        Vector arrowUp = m_arrowUp->GetPosition();
        Vector arrowDown = m_arrowDown->GetPosition();
        if (e->X >= arrowUp.x + parent.x  && e->X <= parent.x + arrowUp.x + m_arrowUp->GetWidth() && e->Y >= arrowUp.y + parent.y && e->Y <= parent.y + arrowUp.y + m_arrowUp->GetHeight())
        {
            //m_arrowUp->MouseEvent(e);
            _MoveScrollUp();
        }
        else if (e->X >= arrowDown.x + parent.x && e->X <= parent.x + arrowDown.x + m_arrowDown->GetWidth() && e->Y >= arrowDown.y + parent.y && e->Y <= parent.y + arrowDown.y + m_arrowDown->GetHeight())
        {
            //m_arrowDown->MouseEvent(e);
            _MoveScrollDown();
        }
        m_lastMouse = *e;
        m_isDragging = false;
    }
}

void GUI_ScrollBar::KeyPress (KeyE *e)
{
}
/*
// TODO: Replace scroll height with view area height
int GUI_ScrollBar::GetScrollBarPosition (void)
{
    if (!m_scroll)
    {
        return 0;
    }
    int steps = m_height-27-m_scroll->GetHeight();
    return (m_scroll->GetPosition().y-13)*(m_maxSize-m_scroll->GetHeight())/steps;
}
*/

int GUI_ScrollBar::GetScrollBarPosition (void)
{
    if (!m_scroll)
    {
        return 0;
    }
    int steps = m_height-30-m_scroll->GetHeight();
    return (m_scroll->GetPosition().y-14)*(m_maxSize-m_height)/steps;
}

void GUI_ScrollBar::_MoveScrollUp (void)
{
    if (!m_scroll)
    {
        return;
    }
    Vector position = m_scroll->GetPosition();
    position.y--;
    int height = m_scroll->GetHeight();
    if (position.y > 13 && position.y + height < m_height - 13)
    {
        m_scroll->SetPosition(position);
        m_scroll->SetStyle(BUTTON_SCROLL);
    }
}

void GUI_ScrollBar::_MoveScrollDown (void)
{
    if (!m_scroll)
    {
        return;
    }
    Vector position = m_scroll->GetPosition();
    position.y++;
    int height = m_scroll->GetHeight();
    if (position.y > 13 && position.y + height < m_height - 13)
    {
        m_scroll->SetPosition(position);
        m_scroll->SetStyle(BUTTON_SCROLL);
    }
}