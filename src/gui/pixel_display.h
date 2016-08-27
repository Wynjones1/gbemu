#ifndef PIXEL_DISPLAY_H
#define PIXEL_DISPLAY_H
#include <array>
#include "keys.h"

int OPENGL_SETTINGS[] = {
    WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0
};

class PixelDisplay : public wxGLCanvas
{
public:
    PixelDisplay(wxFrame *parent)
    : wxGLCanvas(parent, wxID_ANY, OPENGL_SETTINGS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
    , context(this)
    , colour({1.0, 0.0, 0.0})
    {
    }

    void setup()
    {
        wxGLCanvas::SetCurrent(context);
        glClearColor(0.3, 0.3, 0.3, 1.0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    void render(wxPaintEvent &evt)
    {
        if(!IsShown()) return;

        setup();

        glBegin(GL_TRIANGLES);
            glColor3fv(&colour[0]);
            glVertex3f(0,0,0);
            glVertex3f(1,1,0);
            glVertex3f(1,0,0);
        glEnd();

        SwapBuffers();
    }

    void idle(wxIdleEvent &ev)
    {
        Refresh(false);
    }

    void resize(wxSizeEvent &ev)
    {
        glViewport(0, 0, GetSize().x, GetSize().y);
    }

    void key_pressed(wxKeyEvent &ev)
    {
        if(ev.GetKeyCode() == 'W')
            key_state[KEY_W] = true;
        else if(ev.GetKeyCode() == 'A')
            key_state[KEY_A] = true;
        else if(ev.GetKeyCode() == 'S')
            key_state[KEY_S] = true;
        else if(ev.GetKeyCode() == 'D')
            key_state[KEY_D] = true;
        else if(ev.GetKeyCode() == 'I')
            key_state[KEY_I] = true;
        else if(ev.GetKeyCode() == 'J')
            key_state[KEY_J] = true;
        else if(ev.GetKeyCode() == 'F')
            key_state[KEY_F] = true;
        else if(ev.GetKeyCode() == 'G')
            key_state[KEY_G] = true;
        else if(ev.GetKeyCode() == WXK_SPACE)
            key_state[KEY_SPACE] = true;

    }

    void key_released(wxKeyEvent &ev)
    {
        if(ev.GetKeyCode() == 'W')
            key_state[KEY_W] = false;
        else if(ev.GetKeyCode() == 'A')
            key_state[KEY_A] = false;
        else if(ev.GetKeyCode() == 'S')
            key_state[KEY_S] = false;
        else if(ev.GetKeyCode() == 'D')
            key_state[KEY_D] = false;
        else if(ev.GetKeyCode() == 'I')
            key_state[KEY_I] = false;
        else if(ev.GetKeyCode() == 'J')
            key_state[KEY_J] = false;
        else if(ev.GetKeyCode() == 'F')
            key_state[KEY_F] = false;
        else if(ev.GetKeyCode() == 'G')
            key_state[KEY_G] = false;
        else if(ev.GetKeyCode() == WXK_SPACE)
            key_state[KEY_SPACE] = false;
    }

    wxGLContext context;
    std::array<float, 3> colour;
    std::array<bool, NUM_KEYS> key_state;

    DECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(PixelDisplay, wxGLCanvas)
    EVT_PAINT(PixelDisplay::render)
    EVT_KEY_UP(PixelDisplay::key_released)
    EVT_KEY_DOWN(PixelDisplay::key_pressed)
    EVT_IDLE(PixelDisplay::idle)
wxEND_EVENT_TABLE()

#endif
