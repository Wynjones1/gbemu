#ifndef PIXEL_DISPLAY_H
#define PIXEL_DISPLAY_H
#include <array>

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
        static int v;
        switch(v)
        {
        case 0:
            colour = {1, 0, 0};
            break;
        case 1:
            colour = {0, 1, 0};
            break;
        default:
            colour = {0, 0, 1};
            break;
        }
        v = (v + 1) % 3;
    }

    void key_released(wxKeyEvent &ev)
    {}

    wxGLContext context;
    std::array<float, 3> colour;

    DECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(PixelDisplay, wxGLCanvas)
    EVT_PAINT(PixelDisplay::render)
    EVT_KEY_UP(PixelDisplay::key_released)
    EVT_KEY_DOWN(PixelDisplay::key_pressed)
    EVT_IDLE(PixelDisplay::idle)
wxEND_EVENT_TABLE()

#endif
