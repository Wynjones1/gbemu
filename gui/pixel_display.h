#ifndef PIXEL_DISPLAY_H
#define PIXEL_DISPLAY_H
#include <array>
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "keys.h"

int OPENGL_SETTINGS[] = {
    WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0
};

constexpr unsigned int W = 160;
constexpr unsigned int H = 144;

class PixelDisplay : public wxGLCanvas
{
public:
    PixelDisplay(wxFrame *parent, cpu_state_t *cpu_)
    : wxGLCanvas(parent, wxID_ANY, OPENGL_SETTINGS, wxDefaultPosition, wxSize(W, H), wxFULL_REPAINT_ON_RESIZE)
    , context(this)
    , colour({1.0, 0.0, 0.0})
    , cpu(cpu_)
    , thread(this, cpu)
    {
        thread.Run();
    }

    void setup()
    {
        wxGLCanvas::SetCurrent(context);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glDisable(GL_BLEND);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, display_framebuffer(cpu->display));
        resize();
    }

    void render(wxPaintEvent &evt)
    {
        if(!IsShown()) return;

        setup();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLE_FAN);
            glColor3f(1,1,1);
            glTexCoord2f(0.0, 1.0);  glVertex2f(-1,-1);
            glTexCoord2f(0.0, 0.0);  glVertex2f(-1,1);
            glTexCoord2f(1.0, 0.0);  glVertex2f(1,1);
            glTexCoord2f(1.0, 1.0);  glVertex2f(1,-1);
        glEnd();

        SwapBuffers();
    }

    void idle(wxIdleEvent &ev)
    {
        Refresh(false);
    }


    void resize()
    {
        auto x = GetSize().x;
        auto y = GetSize().y;
        auto scale_x = (double)x / (double)W;
        auto scale_y = (double)y / (double)H;
        if (scale_x > scale_y)
        {
            auto x_new = W * scale_y;
            glViewport((x - x_new)/2, 0, x_new, y);
        }
        else
        {
            auto y_new = H * scale_x;
            glViewport(0, (y - y_new) / 2, x, y_new);
        }
    }

    void resize(wxSizeEvent &ev)
    {
        resize();
    }
    
    void key_pressed(wxKeyEvent &ev)
    {
        if(ev.GetKeyCode() == 'W')
        {
            cpu->memory->dpad.up   = 0;
            cpu->memory->dpad.down = 1;
        }
        else if(ev.GetKeyCode() == 'A')
        {
            cpu->memory->dpad.left  = 0;
            cpu->memory->dpad.right = 1;
        }
        else if(ev.GetKeyCode() == 'S')
        {
            cpu->memory->dpad.down = 0;
            cpu->memory->dpad.up   = 1;
        }
        else if(ev.GetKeyCode() == 'D')
        {
            cpu->memory->dpad.right = 0;
            cpu->memory->dpad.left  = 1;
        }
        else if(ev.GetKeyCode() == 'I')
        {
            cpu->memory->buttons.a = 0;
        }
        else if(ev.GetKeyCode() == 'J')
        {
            cpu->memory->buttons.b = 0;
        }
        else if(ev.GetKeyCode() == 'F')
        {
            cpu->memory->buttons.select = 0;
        }
        else if(ev.GetKeyCode() == 'G')
        {
            cpu->memory->buttons.start = 0;
        }
        else if(ev.GetKeyCode() == WXK_ESCAPE)
        {
            exit(0);
        }
    }

    void key_released(wxKeyEvent &ev)
    {
        if(ev.GetKeyCode() == 'W')
        {
            cpu->memory->dpad.up   = 1;
        }
        else if(ev.GetKeyCode() == 'A')
        {
            cpu->memory->dpad.left  = 1;
        }
        else if(ev.GetKeyCode() == 'S')
        {
            cpu->memory->dpad.down = 1;
        }
        else if(ev.GetKeyCode() == 'D')
        {
            cpu->memory->dpad.right = 1;
        }
        else if(ev.GetKeyCode() == 'I')
        {
            cpu->memory->buttons.a = 1;
        }
        else if(ev.GetKeyCode() == 'J')
        {
            cpu->memory->buttons.b = 1;
        }
        else if(ev.GetKeyCode() == 'F')
        {
            cpu->memory->buttons.select = 1;
        }
        else if(ev.GetKeyCode() == 'G')
        {
            cpu->memory->buttons.start = 1;
        }
    }

    GLuint texture;
    std::array<uint8_t, W * H * 3> framebuffer;
    wxGLContext context;
    std::array<float, 3> colour;
    std::array<bool, NUM_KEYS> key_state;
    cpu_state_t  *cpu;

    class Thread : public wxThread
    {
    public:
        Thread(PixelDisplay *parent_, cpu_state_t *cpu_)
            : wxThread(wxTHREAD_DETACHED)
            , parent(parent_)
            , cpu(cpu_)
        {}
        
        ExitCode Entry()
        {
            cpu_start(cpu);
            return nullptr;
        }

        PixelDisplay *parent;
        cpu_state_t  *cpu;
    }thread;


    DECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(PixelDisplay, wxGLCanvas)
    EVT_PAINT(PixelDisplay::render)
    EVT_KEY_UP(PixelDisplay::key_released)
    EVT_KEY_DOWN(PixelDisplay::key_pressed)
    EVT_IDLE(PixelDisplay::idle)
    EVT_SIZE(PixelDisplay::resize)
wxEND_EVENT_TABLE()

#endif
