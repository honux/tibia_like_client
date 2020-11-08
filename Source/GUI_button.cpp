#include "GUI_button.h"
#include "window.h"
#include "font.h"
#include "functor.h"
#include "events.h"

extern Window *g_window;
extern Font *g_font;

GUI_Button::GUI_Button (void)
: GUI_Element(), m_nameStr(NULL)
{
    m_content = new GUI_RectangleList(5, 3);
}

GUI_Button::GUI_Button (int width, int height, Vector position, utils::Functor* functor, int style)
: GUI_Element(width, height, position), m_functor(functor), m_nameStr(NULL)
{
    m_content = new GUI_RectangleList(5, 3);

    if (style >= 0)
    {
        SetStyle(style);
    }
}

GUI_Button::~GUI_Button (void)
{
    delete[] m_nameStr;
    delete m_content;
    delete m_functor;
}

void GUI_Button::Render (Vector pos, ECOLOR *style)
{
    ECOLOR color = (style)?*style:m_color;
    g_window->RenderList(m_content, color, pos);

    if (m_nameStr)
    {
        text.Render(pos, &color);
    }
}

void GUI_Button::MouseEvent (MouseE *e)
{
    if (e->State == EMOUSE_BUTTON_UP && e->Button == EMOUSE_LEFT && m_functor)
    {
        (*m_functor)();
    }
}

void GUI_Button::KeyPress (KeyE *e)
{
}

void GUI_Button::Add (Vector2 dimensions, Vector2 texturePos, int pos)
{
    m_content->Add(dimensions, texturePos, pos);
}

void GUI_Button::SetCallBack (utils::Functor* functor)
{
    m_functor = functor;
}

void GUI_Button::SetName (char* name, Vector pos)
{
    delete[] m_nameStr;
    
    m_nameStr = new char[strlen(name)+1];
    strcpy( m_nameStr, name );

    text.SetString(m_nameStr);
    text.SetPosition(pos);
}

const char* GUI_Button::GetName (void)
{
    return m_nameStr;
}

// Some default styles to do it faster, eh?
void GUI_Button::SetStyle (short id)
{
    switch (id)
    {
        case DEFAULT_BUTTON:
        {
            m_content->Add(Vector2(m_position.x, m_position.y, m_position.x+2, m_position.y+m_height),
                Vector2(57.0/256.0, 71.0/128.0, 59./256.0, 95.0/128.0), 0);

            m_content->Add(Vector2(m_position.x+2, m_position.y, m_position.x+m_width-2, m_position.y+m_height),
                Vector2(59.0/256.0, 71.0/128.0, 69./256.0, 95.0/128.0), 1);
     
            m_content->Add(Vector2(m_position.x+m_width-2, m_position.y, m_position.x+m_width, m_position.y+m_height),
                Vector2(69.0/256.0, 71.0/128.0, 71./256.0, 95.0/128.0), 2);
        }
        break;

        case BUTTON_CLOSE:
        {
            m_content->SetTexture(4);
            m_content->Add(Vector2(m_position.x, m_position.y, m_position.x+m_width, m_position.y+m_height),
                        Vector2(35.0/256.0, 90.0/128.0, 51.0/256.0, 104.0/128.0), 0);
        }
        break;

        case BUTTON_MINUS:
        {
            m_content->SetTexture(4);
            m_content->Add(Vector2(m_position.x, m_position.y, m_position.x+m_width, m_position.y+m_height),
                        Vector2(0.0/256.0, 90.0/128.0, 16.0/256.0, 104.0/128.0), 0);
        }
        break;

        case BUTTON_PLUS:
        {
            m_content->SetTexture(4);
            m_content->Add(Vector2(m_position.x+1, m_position.y, m_position.x+m_width+1, m_position.y+m_height),
                        Vector2(18.0/256.0, 90.0/128.0, 34.0/256.0, 104.0/128.0), 0);
        }
        break;

        case BUTTON_ARROW_UP:
        {
            m_content->SetTexture(4);
            m_content->Add(Vector2(m_position.x, m_position.y, m_position.x+m_width, m_position.y+m_height),
                        Vector2(52.0/256.0, 90.0/128.0, 68.0/256.0, 104.0/128.0), 0);
        }
        break;

        case BUTTON_ARROW_DOWN:
        {
            m_content->SetTexture(4);
            m_content->Add(Vector2(m_position.x, m_position.y, m_position.x+m_width, m_position.y+m_height),
                        Vector2(69.0/256.0, 90.0/128.0, 85.0/256.0, 104.0/128.0), 0);
        }
        break;

        case BUTTON_SCROLL:
        {
            int height = (m_height-17)/2;
            m_content->SetTexture(4);
            m_content->Add(Vector2(m_position.x, m_position.y, m_position.x+m_width, m_position.y+2),
                        Vector2(38.0/256.0, 87.0/128.0, 52.0/256.0, 89.0/128.0), 0);
            m_content->Add(Vector2(m_position.x, m_position.y+2, m_position.x+m_width, m_position.y+2+height),
                        Vector2(38.0/256.0, 84.0/128.0, 52.0/256.0, 86.0/128.0), 1);
            m_content->Add(Vector2(m_position.x, m_position.y+height+2, m_position.x+m_width, m_position.y+height+15),
                        Vector2(38.0/256.0, 70.0/128.0, 52.0/256.0, 83.0/128.0), 2);
            m_content->Add(Vector2(m_position.x, m_position.y+height+15, m_position.x+m_width, m_position.y+15+2*height),
                        Vector2(38.0/256.0, 84.0/128.0, 52.0/256.0, 86.0/128.0), 3);
            m_content->Add(Vector2(m_position.x, m_position.y+15+2*height, m_position.x+m_width, m_position.y+17+2*height),
                        Vector2(38.0/256.0, 65.0/128.0, 52.0/256.0, 67.0/128.0), 4);
        }
        break;

        default:
        break;
    }
}