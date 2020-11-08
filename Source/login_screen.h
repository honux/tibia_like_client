#pragma once

#include "events.h"
#include "screen.h"
#include "GUI_rectangle.h"
#include "GUI_button.h"
#include "GUI_box.h"
#include "GUI_textbox.h"
#include "GUI_container.h"
#include "charlist.h"

class LoginScreen: public Screen
{
public:
    LoginScreen(void);
    ~LoginScreen(void);

    void KeyEvent (KeyE *e);
    void MouseEvent (MouseE *e);

    void Resize (int width, int height);
    void RemoveElement (GUI_Element* thing);

private:
    void _DisplayLogin (void);
    void _DisplayAbout (void);
    void _DisplayContainer (void);
    void _Exit (void);
    void _Login (void);
private:
    /* Some variable holders, to access it faster :) */
    GUI_Rectangle *m_background;
    GUI_Box *m_login;
    GUI_TextBox *m_account, *m_password;

    int m_width;
    int m_height;
};
