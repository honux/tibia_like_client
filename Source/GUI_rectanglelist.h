#pragma once

#include <windows.h>

class GUI_RectangleList
{
public:
    GUI_RectangleList (void)
    : m_size(0), m_texture(0), m_cursor(0), m_dimensions(NULL), m_textPos(NULL)
    {
    }

    GUI_RectangleList (unsigned size, unsigned texture)
    : m_size(size), m_texture(texture), m_cursor(0)
    {
        m_dimensions = new Vector2[size];
        m_textPos = new Vector2[size];
    }
    
    ~GUI_RectangleList (void)
    {
        delete[] m_dimensions;
        delete[] m_textPos;
    }

    unsigned Add (Vector2 dimensions, Vector2 textPos, int pos = -1)
    {
        if (pos == -1)
        {
            if (m_cursor == m_size)
            {
                return 0;
            }
            pos = m_cursor;
        }

        *(m_dimensions+pos) = dimensions;
        *(m_textPos+pos) = textPos;
        return (pos==m_cursor)?++m_cursor:pos;
    }

    inline Vector2* GetDimensions (void)
    {
        return m_dimensions;
    }

    inline Vector2* GetTexturePosition (void)
    {
        return m_textPos;
    }
    
    inline unsigned GetTexture (void)
    {
        return m_texture;
    }
    
    inline unsigned GetSize (void)
    {
        return m_cursor;
    }

    inline void SetTexture (unsigned texture)
    {
        m_texture = texture;
    }

    inline void SetDimensions (unsigned size)
    {
        delete[] m_dimensions;
        delete[] m_textPos;
        m_dimensions = new Vector2[size];
        m_textPos = new Vector2[size];
    }

private:
    unsigned m_size;
    unsigned m_texture;
    unsigned m_cursor;

    Vector2 *m_dimensions;
    Vector2 *m_textPos;
};