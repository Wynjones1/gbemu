#ifndef PIXEL_DISPLAY_H
#define PIXEL_DISPLAY_H
#include <array>
#include "keys.h"

int OPENGL_SETTINGS[] = {
    WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0
};

constexpr unsigned int W = 10;
constexpr unsigned int H = 10;

class PixelDisplay : public wxGLCanvas
{
public:
    PixelDisplay(wxFrame *parent)
    : wxGLCanvas(parent, wxID_ANY, OPENGL_SETTINGS, wxDefaultPosition, wxSize(W, H), wxFULL_REPAINT_ON_RESIZE)
    , context(this)
    , colour({1.0, 0.0, 0.0})
    , thread(this)
    {
        thread.Run();
    }

    void setup()
    {
        wxGLCanvas::SetCurrent(context);
        glClearColor(0.3, 0.3, 0.3, 1.0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glDisable(GL_BLEND);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)framebuffer.data());

    }

    void render(wxPaintEvent &evt)
    {
        if(!IsShown()) return;

        setup();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_TRIANGLE_FAN);
            glColor3f(1,1,1);
            glTexCoord2f(0.0, 0.0);  glVertex2f(-1,-1);
            glTexCoord2f(0.0, 1.0);  glVertex2f(-1,1);
            glTexCoord2f(1.0, 1.0);  glVertex2f(1,1);
            glTexCoord2f(1.0, 0.0);  glVertex2f(1,-1);
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
        else if(ev.GetKeyCode() == WXK_ESCAPE)
            exit(0);

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

    GLuint texture;
    std::array<uint8_t, W * H * 3> framebuffer;
    wxGLContext context;
    std::array<float, 3> colour;
    std::array<bool, NUM_KEYS> key_state;

    class Thread : public wxThread
    {
    public:
        Thread(PixelDisplay *parent_)
            : wxThread(wxTHREAD_DETACHED)
            , parent(parent_)
        {}
        
        ExitCode Entry()
        {
            while (1)
            {
                for (auto &x : parent->framebuffer)
                {
                    x = rand();
                }
            }
            return nullptr;
        }

        PixelDisplay *parent;
    }thread;

    DECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(PixelDisplay, wxGLCanvas)
    EVT_PAINT(PixelDisplay::render)
    EVT_KEY_UP(PixelDisplay::key_released)
    EVT_KEY_DOWN(PixelDisplay::key_pressed)
    EVT_IDLE(PixelDisplay::idle)
wxEND_EVENT_TABLE()

#endif
