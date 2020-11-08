#include "GUI_textbox.h"
#include "window.h"
#include "font.h"
#include "screen.h"

extern Window *g_window;
extern Font *g_font;
extern Screen *g_screen;

GUI_TextBox::GUI_TextBox (void)
: GUI_Element(), m_curLenght(0), m_cursorPos(0), m_renderCursor(false), m_flags(0), m_anchor(0)
{
    m_content = new GUI_RectangleList(3, 3);

    _UpdatePosition();
}

GUI_TextBox::GUI_TextBox (int width, int height, Vector position, int maxsize, int flags)
: GUI_Element(width, height, position), m_curLenght(0), m_cursorPos(0), m_renderCursor(false), m_flags(0), m_anchor(0)
{
    m_strMaxSize = maxsize;
    m_string = new char[m_strMaxSize+1];
    m_string[0] = '\0';

    m_display = new char[m_strMaxSize + 1];
    m_display[0] = '\0';

    m_content = new GUI_RectangleList(3, 3);

    _UpdatePosition();
}

GUI_TextBox::~GUI_TextBox (void)
{
    delete[] m_string;
    delete[] m_display;
    delete m_content;
}

void GUI_TextBox::_UpdatePosition (void)
{
    /* Aways following this order:
        1 - Left Bar;
        2 - Middle;
        3 - Right Bar;
    */
    m_content->Add(Vector2(m_position.x, m_position.y, m_position.x+1, m_position.y+m_height),
        Vector2(72.0/256.0, 36.0/128.0, 73./256.0, 65.0/128.0), 0);
    m_content->Add(Vector2(m_position.x+1, m_position.y, m_position.x+m_width-1, m_position.y+m_height),
        Vector2(74.0/256.0, 36.0/128.0, 87./256.0, 65.0/128.0), 1);
    m_content->Add(Vector2(m_position.x+m_width-1, m_position.y, m_position.x+m_width, m_position.y+m_height),
        Vector2(87.0/256.0, 36.0/128.0, 88./256.0, 65.0/128.0), 2);
    /*
    text.SetString(m_nameStr);
    text.SetPosition(Vector(m_position.x+m_width/2-(g_font->GetSize(m_nameStr)/2), m_position.y+(m_height/2)-5));
    */
}

void GUI_TextBox::Render (Vector pos, ECOLOR *style)
{
    ECOLOR color = (style)?*style:m_color;
    g_window->RenderList(m_content, color, pos);

    g_window->RenderText(Vector(pos.x+m_position.x+2, pos.y+m_position.y-4+(m_height>>1)), m_display, color);
    
    if (m_renderCursor)
    {
        // temporary cursor render
        int size = g_font->GetSize(m_display, m_cursorPos-m_anchor);
        g_window->RenderRectangle(Vector2(m_position.x+size+pos.x, m_position.y+pos.y, m_position.x+pos.x+size+1, m_position.y+pos.y+16), color, 3,
            Vector2(41.0/256.0, 71.0/128.0, 42.0/256.0, 95.0/128.0) );
    }
}

void GUI_TextBox::MouseEvent (MouseE *e)
{
    //g_screen->SetFocus(this);
    if (m_curLenght != 0)
    {
    /*
        int last = g_font->GetLastSize(m_string, pos.x - m_position.x);
        int length = g_font->GetSize(m_string, last);
        int letter = g_font->GetCharSize(m_string[last]);
        if ((m_position.x + length + (letter>>1)) +2 >= pos.x )
        {
            m_cursorPos = last;
        }
        else
        {
            m_cursorPos = (last+1 <= m_curLenght)?last+1:m_curLenght;
        }
    */
    }
}

void GUI_TextBox::KeyPress (KeyE *e)
{
    if (!e->PressedDown)
        return;
    switch (e->Key)
    {
        case KEY_BACK:
            _RemoveLeftChar();
        break;

        case KEY_DELETE:
            _RemoveRightChar();
        break;

        case KEY_RIGHT:
            if (m_cursorPos != m_strMaxSize && m_string[m_cursorPos] != '\0')
            {
                m_cursorPos++;
            }
        break;

        case KEY_LEFT:
            if (m_cursorPos != 0)
            {
                m_cursorPos--;
            }
        break;

        default:
            if (e->Char != 0)
            {
                _HandleAccentuation((unsigned char)e->Char);
            }
        break;
    }
    _UpdateText();
}

void GUI_TextBox::_HandleAccentuation (unsigned char key)
{
    static unsigned char last = 0;
    if (key == 180 || key == '`' || key == '~' || key == '^' || key == 168)
    {
        if (last == 180 || last == '`' || last == '~' || last == 168)
        {
            _AddChar(last);
            _AddChar(key);
            last = 0;
            return;
        }
        else if (last == '^')
        {
            _AddChar(136);
            _AddChar(136);
            last = 0;
            return;
        }
        last = key;
    }
    else
    {
        if (last == 180)
        {
            switch (key)
            {
                case 'a':
                    _AddChar('á');
                break;
                case 'A':
                    _AddChar('Á');
                break;
                case 'e':
                    _AddChar('é');
                break;
                case 'E':
                    _AddChar('É');
                break;
                case 'i':
                    _AddChar('í');
                break;
                case 'I':
                    _AddChar('Í');
                break;
                case 'o':
                    _AddChar('ó');
                break;
                case 'O':
                    _AddChar('Ó');
                break;
                case 'u':
                    _AddChar('ú');
                break;
                case 'U':
                    _AddChar('Ú');
                break;
                case 'y':
                    _AddChar('ý');
                break;
                case 'Y':
                    _AddChar('Ý');
                break;
                default:
                    _AddChar(last);
                    _AddChar(key);
                break;
            }
            last = 0;
        }
        else if (last == '`')
        {
            switch (key)
            {
                case 'a':
                    _AddChar('à');
                break;
                case 'A':
                    _AddChar('À');
                break;
                case 'e':
                    _AddChar('è');
                break;
                case 'E':
                    _AddChar('È');
                break;
                case 'i':
                    _AddChar('ì');
                break;
                case 'I':
                    _AddChar('Ì');
                break;
                case 'o':
                    _AddChar('ò');
                break;
                case 'O':
                    _AddChar('Ò');
                break;
                case 'u':
                    _AddChar('ù');
                break;
                case 'U':
                    _AddChar('Ù');
                break;
                default:
                    _AddChar(last);
                    _AddChar(key);
                break;
            }
        }
        else if (last == '^')
        {
            switch (key)
            {
                case 'a':
                    _AddChar('â');
                break;
                case 'A':
                    _AddChar('Â');
                break;
                case 'e':
                    _AddChar('ê');
                break;
                case 'E':
                    _AddChar('Ê');
                break;
                case 'i':
                    _AddChar('î');
                break;
                case 'I':
                    _AddChar('Î');
                break;
                case 'o':
                    _AddChar('ô');
                break;
                case 'O':
                    _AddChar('Ô');
                break;
                case 'u':
                    _AddChar('û');
                break;
                case 'U':
                    _AddChar('Û');
                break;
                default:
                    _AddChar(last);
                    _AddChar(key);
                break;
            }
        }
        else if (last == '~')
        {
            switch (key)
            {
                case 'a':
                    _AddChar('ã');
                break;
                case 'A':
                    _AddChar('Ã');
                break;
                case 'o':
                    _AddChar('õ');
                break;
                case 'O':
                    _AddChar('Õ');
                break;
                case 'n':
                    _AddChar('ñ');
                break;
                case 'N':
                    _AddChar('Ñ');
                break;
                default:
                    _AddChar(last);
                    _AddChar(key);
                break;
            }
        }
        else if (last == 168)
        {
            switch (key)
            {
                case 'a':
                    _AddChar('ä');
                break;
                case 'A':
                    _AddChar('Ä');
                break;
                case 'e':
                    _AddChar('ë');
                break;
                case 'E':
                    _AddChar('Ë');
                break;
                case 'i':
                    _AddChar('ï');
                break;
                case 'I':
                    _AddChar('Ï');
                break;
                case 'o':
                    _AddChar('ö');
                break;
                case 'O':
                    _AddChar('Ö');
                break;
                case 'u':
                    _AddChar('ü');
                break;
                case 'U':
                    _AddChar('Ü');
                break;
                case 'y':
                    _AddChar('ÿ');
                break;
                default:
                    _AddChar(last);
                    _AddChar(key);
                break;
            }
        }
        else
        {
            _AddChar(key);
        }
        last = key;
    }
}

bool GUI_TextBox::_AddChar (char letter)
{
    if (m_curLenght == m_strMaxSize || m_cursorPos == m_strMaxSize || g_font->GetCharSize(letter) == 0)
    {
        return false;
    }
    char next = letter;
    for (int i = m_strMaxSize+1; i != m_cursorPos; i--)
    {
        m_string[i] = m_string[i-1];
    }
    m_string[m_cursorPos] = letter;
    m_cursorPos++;
    m_curLenght++;
    return true;
}

bool GUI_TextBox::_RemoveLeftChar (void)
{
    if (m_cursorPos == 0 || m_curLenght == 0)
    {
        return true;
    }
    for (int i = m_cursorPos; i < m_strMaxSize+1; i++)
    {
        m_string[i-1] = m_string[i];
        if (m_string[i] = '\0')
        {
            break;
        }
    }
    m_cursorPos--;
    m_string[m_curLenght] = '\0';
    m_curLenght--;
    return true;
}

bool GUI_TextBox::_RemoveRightChar (void)
{
    if (m_string[m_cursorPos] == '\0')
    {
        return true;
    }
    int i = m_cursorPos;
    while (m_string[i] != '\0')
    {
        m_string[i] = m_string[i+1];
        i++;
    }
    m_curLenght--;

    return true;
}

void GUI_TextBox::_UpdateText (void)
{
    /* Cursor is outside the screen to the right */
    if (m_cursorPos > m_drawEnd)
    {
        m_anchor = 0;
        m_drawEnd = m_cursorPos;
        int lenght = 0;
        int cursor = m_cursorPos;
        for(;;)
        {
            if (cursor <= 0)
            {
                break;
            }
            int letter = g_font->GetCharSize(m_string[cursor]);
            if (lenght + letter > m_width-8)
            {
                break;
            }
            lenght += letter;
            cursor--;
        }

        m_anchor = (m_cursorPos == m_strMaxSize)?++cursor:cursor;
    }
    /* Cursor is outside the screen to the left */
    else if (m_cursorPos < m_anchor)
    {
        m_anchor = m_drawEnd = m_cursorPos;
        int lenght = 0;
        for(;;)
        {
            if (m_string[m_drawEnd] == '\0')
            {
                break;
            }
            int letter = g_font->GetCharSize(m_string[m_drawEnd]);
            if (lenght + letter > m_width-4)
            {
                break;
            }
            lenght += letter;
            m_drawEnd++;
        }
    }

    if (HasFlag(FLAG_ISPASSWORD))
    {
        for (int i = 0; i < m_drawEnd-m_anchor; i++)
        {
            m_display[i] = '*';
        }
    }
    else
    {
        for (int i = 0; i < m_drawEnd-m_anchor; i++)
        {
            m_display[i] = m_string[i+m_anchor];
        }
    }
    m_display[m_drawEnd-m_anchor] = '\0';
}