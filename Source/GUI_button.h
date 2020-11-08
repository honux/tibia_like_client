#pragma once
#include "GUI_element.h"
#include "GUI_rectanglelist.h"
#include "GUI_text.h"

#include "window.h"
extern Window *g_window;

#define DEFAULT_BUTTON      0
#define BUTTON_CLOSE        1
#define BUTTON_MINUS        2
#define BUTTON_PLUS         3
#define BUTTON_ARROW_UP     4
#define BUTTON_ARROW_DOWN   5
#define BUTTON_SCROLL       6

namespace utils
{
    class Functor;
}

class GUI_Button: public GUI_Element
{
public:
    GUI_Button (void);
    GUI_Button::GUI_Button (int width, int height, Vector position, utils::Functor* functor, int style = -1);
    ~GUI_Button (void);
     
    void Render (Vector pos = Vector(), ECOLOR *style = NULL);
    void MouseEvent (MouseE *e);
    void KeyPress (KeyE *e);
   
    void Add (Vector2 dimensions, Vector2 texturePos, int pos);
    void SetName (char* name, Vector pos);
    void SetCallBack (utils::Functor* functor);
    void SetStyle (short id);
    const char* GetName (void);

private:

private:
    char *m_nameStr;
    GUI_RectangleList* m_content;
    GUI_Text text;
    utils::Functor* m_functor;
};