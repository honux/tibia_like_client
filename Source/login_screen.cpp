#include "login_screen.h"
#include "definitions.h"
#include "functor.h"
#include "font.h"

extern Font *g_font;

LoginScreen::LoginScreen (void)
: m_login(NULL)
{
    m_displayFps = false;
    m_selected = NULL;
    m_dragging = NULL;
    m_curFocus = NULL;
    m_locked = NULL;

    GUI_Rectangle *element = new GUI_Rectangle(800, 600, Vector(0,0), 1, Vector2(0.0, (424.0f/1024.0f), (800.0f/1024.0f), 1.0));
    m_background = element;
    m_screenElement.push_back(element);

    GUI_Button *button = new GUI_Button(100, 24, Vector(60, 430), utils::Bind(&LoginScreen::_DisplayLogin, this), 0);
    button->SetName("Enter Game", Vector(60+50-(g_font->GetSize("Enter Game")/2), 430+12-5));
    button->SetSelectable(true);
    m_screenElement.push_back(button);

    button = new GUI_Button(100, 24, Vector(60, 470), utils::Bind(&LoginScreen::_DisplayContainer, this), 0);
    button->SetName("Options", Vector(60+50-(g_font->GetSize("Options")/2), 470+12-5));
    button->SetSelectable(true);
    m_screenElement.push_back(button);

    button = new GUI_Button(100, 24, Vector(60, 510), utils::Bind(&LoginScreen::_DisplayAbout, this), 0);
    button->SetName("About", Vector(60+50-(g_font->GetSize("About")/2), 510+12-5));
    button->SetSelectable(true);
    m_screenElement.push_back(button);

    button = new GUI_Button(100, 24, Vector(60, 550), utils::Bind(&LoginScreen::_Exit, this), 0);
    button->SetName("Exit", Vector(60+50-(g_font->GetSize("Exit")/2), 550+12-5));
    button->SetSelectable(true);
    m_screenElement.push_back(button);
}

LoginScreen::~LoginScreen(void)
{
    /* delete elements?*/
}

void LoginScreen::KeyEvent (KeyE *e)
{
    if (e->Key == KEY_F1 && e->PressedDown && e->Shift)
    {
        m_displayFps = !m_displayFps;
    }
}

void LoginScreen::MouseEvent (MouseE *e)
{
    // The screen has some element that MUST be on top until it complete it's execution
    if (m_locked)
    {
        if (e->X >= m_locked->m_position.x && e->X <= m_locked->m_position.x+m_locked->m_width &&
            e->Y >= m_locked->m_position.y && e->Y <= m_locked->m_position.y+m_locked->m_height)
        {
            m_locked->MouseEvent(e);
        }
    }
    else
    {
        GUI_Element *selected = NULL;
        for (std::list<GUI_Element*>::reverse_iterator it = m_screenElement.rbegin(); (it != m_screenElement.rend()) && e->State != EMOUSE_MOVE; it++)
        {
            if (e->X >= (*it)->m_position.x && e->X <= (*it)->m_position.x+(*it)->m_width &&
                e->Y >= (*it)->m_position.y && e->Y <= (*it)->m_position.y+(*it)->m_height)
            {
                selected = (*it);
                break;
            }
        }

        if (e->State == EMOUSE_BUTTON_DOWN)
        {
            if (selected && selected != m_background)
            {
                m_screenElement.remove(selected);
                m_screenElement.push_back(selected);
                m_selected = selected;
                m_selected->MouseEvent(e);
            }
        }
        else if (e->State == EMOUSE_MOVE)
        {
            if (m_selected)
            {
                m_selected->MouseEvent(e);
            }
        }
        else if (e->State == EMOUSE_BUTTON_UP)
        {
            if (m_selected)
            {
                m_selected = NULL;
            }
            selected->MouseEvent(e);
        }
        else
        {
            selected->MouseEvent(e);
        }
    }
}

void LoginScreen::Resize (int width, int height)
{
    m_width = width;
    m_height = height;
    m_background->SetSize(width, height);
}

void LoginScreen::RemoveElement (GUI_Element* thing)
{
    m_screenElement.remove(thing);
    if (m_selected == thing)
    {
        m_selected = NULL;
    }
    if (thing == m_login)
    {
        m_login = NULL;
        m_curFocus = NULL;
    }
    if (thing == m_locked)
    {
        m_locked = NULL;
    }

    delete thing;
}

/* Buttons functions callback */

void LoginScreen::_DisplayLogin (void)
{
    if (!m_login)
    {
        GUI_Box *box = new GUI_Box(249, 200, Vector((m_width/2)-125, (m_height/2)-75), "Login");
        box->SetSelectable(true);
        {
            GUI_Button *button = new GUI_Button(100, 24, Vector(20, 148), utils::Bind(&LoginScreen::_Login, this), 0);
            button->SetName("Connect", Vector(20+50-(g_font->GetSize("Connect")/2), 148+12-5));
            button->SetSelectable(true);
            box->Push(button);

            button = new GUI_Button(100, 24, Vector(130, 148), utils::Bind(&LoginScreen::RemoveElement, this, (GUI_Element*)box), 0);
            button->SetName("Cancel", Vector(130+50-(g_font->GetSize("Cancel")/2), 148+12-5));
            button->SetSelectable(true);
            box->Push(button);

            GUI_Text *text = new GUI_Text(Vector(10, 30), "Account");
            box->Push(text);

            text = new GUI_Text(Vector(10, 70), "Password");
            box->Push(text);

            GUI_TextBox *account = new GUI_TextBox(130, 20, Vector(10, 42), 50);
            account->SetSelectable(true);
            account->RenderCursor(true);
            box->Push(account);

            m_curFocus = account;
            m_account = account;

            GUI_TextBox *password = new GUI_TextBox(130, 20, Vector(10, 82), 15);
            password->SetSelectable(true);
            password->SetFlag(FLAG_ISPASSWORD);
            box->Push(password);

            m_password = password;
        }
        m_login = box;
        m_screenElement.push_back( box );
    }
}

void LoginScreen::_DisplayAbout (void)
{
    if (!m_locked)
    {
        GUI_Box *box = new GUI_Box(230, 120, Vector((m_width/2)-125, (m_height/2)-75), "About");
        box->SetSelectable(true);
        {
            GUI_Text *text = new GUI_Text(Vector(115-g_font->GetSize("Created by Honux")/2, 40), "Created by Honux");
            box->Push(text);

            GUI_Button *button = new GUI_Button(100, 24, Vector(65, 65), utils::Bind(&LoginScreen::RemoveElement, this, (GUI_Element*)box), 0);
            button->SetName("Ok", Vector(65+50-(g_font->GetSize("Ok")/2), 65+12-5));
            button->SetSelectable(true);
            box->Push(button);
        }
        box->SetColor(ECOLOR(1.0f, 1.0f, 1.0f, 0.5f));

        m_locked = box;
        m_screenElement.push_back( box );
    }
}

void LoginScreen::_Exit (void)
{
    exit(0);
}

void LoginScreen::_DisplayContainer (void)
{
    GUI_Container *container = new GUI_Container(140, 100, Vector(100, 100), 20);
        GUI_Button *button = new GUI_Button(16, 14, Vector(120, 4), utils::Bind(&LoginScreen::RemoveElement, this, (GUI_Element*)container), 1);
        container->PushButton(button);
        button = new GUI_Button(16, 14, Vector(100, 4), NULL, 2);
        button->SetCallBack(utils::Bind(&GUI_Container::ChangeMinimized, container, button));
        container->PushButton(button);
    m_screenElement.push_back( container );
}

/* Other CallBacks */
 void LoginScreen::_Login (void)
 {
    if (m_login)
    {
        char *account = m_account->GetText();
        char *password = m_password->GetText();
        int temp = g_font->GetSize(account);
        /*
        if (account[0] == '\0' || password[0] == '\0')
        {
            if (!m_locked)
            {
                GUI_Box *box = new GUI_Box(230, 120, Vector((m_width/2)-125, (m_height/2)-75), "Error");
                box->SetSelectable(true);
                {
                    GUI_Text *text = new GUI_Text(Vector(115-g_font->GetSize("Please fill all the fields")/2, 40), "Please fill all the fields");
                    box->Push(text);

                    GUI_Button *button = new GUI_Button(100, 24, Vector(65, 65), utils::Bind(&LoginScreen::RemoveElement, this, (GUI_Element*)box), 0);
                    button->SetName("Ok", Vector(65+50-(g_font->GetSize("Ok")/2), 65+12-5));
                    button->SetSelectable(true);
                    box->Push(button);
                }

                m_locked = box;
                m_screenElement.push_back( box );
            }
        }
        else
        */
        {
            CharacterList* charList = new CharacterList(300, 350, Vector((m_width/2)-150, (m_height/2)-175));
            {
                GUI_Button *button = new GUI_Button(100, 24, Vector(25, 300), NULL, 0);
                button->SetName("Ok", Vector(25+50-(g_font->GetSize("Ok")/2), 300+12-5));
                button->SetSelectable(true);
                charList->Push(button);

                button = new GUI_Button(100, 24, Vector(175, 300), utils::Bind(&LoginScreen::RemoveElement, this, (GUI_Element*)charList), 0);
                button->SetName("Cancel", Vector(175+50-(g_font->GetSize("Cancel")/2), 300+12-5));
                button->SetSelectable(true);
                charList->Push(button);
                /*
                button = new GUI_Button(100, 24, Vector(5, 50), NULL, 0);
                button->SetName("New Char", Vector(5+50-(g_font->GetSize("New Char")/2), 50+12-5));
                button->SetSelectable(true);
                charList->Push(button);
                */
            }
            RemoveElement(m_login);
            m_screenElement.push_back(charList);
        }
    }
 }