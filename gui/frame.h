#ifndef GUI_FRAME_H
#define GUI_FRAME_H
#undef min
#undef max
#include <iomanip>
#include "cpu.h"
#include <sstream>
#include "pixel_display.h"
#include "dissassembly.h"
#include "register_window.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
    , cpu(cpu_init())
    , timer(this, 0)
    {
        // Create the menu bar.
        auto file_menu = new wxMenu();
        file_menu->AppendSeparator();
        file_menu->Append(wxID_EXIT);

        auto menu_bar = new wxMenuBar();
        menu_bar->Append(file_menu, "&File");

        SetMenuBar(menu_bar);
        
        // Create the status bar.
        CreateStatusBar();
        SetStatusText("Hello");

        auto display = new PixelDisplay(this, cpu);
        display->Show(true);

        auto register_window = new RegisterWindow(this, cpu);
        register_window->Show(true);

        auto disassembly = new DisassemblyWindow(this, cpu);
        disassembly->Show(true);

        auto sizer = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(display, 1, wxEXPAND);
        sizer->Add(register_window, 1, wxEXPAND);
        sizer->Add(disassembly, 1, wxEXPAND);

        timer.Start(20);

        SetSizer(sizer);
        SetAutoLayout(true);
    }

    void ShowFPS(wxTimerEvent& event)
    {
        static int count;
        std::stringstream ss;
        ss << "%:" << cpu->fps;
        SetStatusText(ss.str().c_str());
    }

    cpu_state_t *cpu;
    wxTimer timer;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_TIMER(0, MainFrame::ShowFPS)
wxEND_EVENT_TABLE()

#endif
