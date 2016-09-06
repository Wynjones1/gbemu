#include <stdio.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "cpu.h"


#include "frame.h"

#include "SDL.h"

class GUI : public wxApp
{
public:
    virtual bool OnInit()
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            printf("\n");
        }
#if _MSC_VER
        cmdline_args.in = "C:\\Users\\stuar\\Dropbox\\GameBoy\\roms\\mario.gb";
#else
        cmdline_args.in = "/home/stuart/Dropbox/GameBoy/roms/mario.gb";
#endif
        auto frame = new MainFrame("Hello", wxPoint(0, 0), wxSize(1300, 1000));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(GUI);
