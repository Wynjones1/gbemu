#include <stdio.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>

enum GUI_ID
{
    GUI_ID_HELLO = 0,
};

#include "pixel_display.h"
#include "frame.h"

class GUI : public wxApp
{
public:
    virtual bool OnInit()
    {
        auto frame = new MainFrame("Hello", wxPoint(0, 0), wxSize(640, 480));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(GUI);
