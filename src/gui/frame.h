#ifndef GUI_FRAME_H
#define GUI_FRAME_H
#include "cpu.h"
#include <sstream>
#include <iomanip>
#include "pixel_display.h"

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
        file_menu->Append(GUI_ID_HELLO, "&Open", "Help string");
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

        text = new wxStaticText(this, 0, wxString("Hello"));
        text->Show(true);

        auto sizer = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(display, 9, wxEXPAND);
        sizer->Add(text, 1, wxEXPAND);

        timer.Start(200);

        SetSizer(sizer);
        SetAutoLayout(true);
    }

    void ShowFPS(wxTimerEvent& event)
    {
        static int count;
        std::stringstream ss;
        ss << "%:" << cpu->fps;
        SetStatusText(ss.str().c_str());

        {
            std::stringstream ss;
            ss << "AF : 0x" << std::hex << std::setw(4) << std::setfill('0') << cpu->af << std::endl
               << "BC : 0x" << std::hex << std::setw(4) << std::setfill('0') << cpu->bc << std::endl
               << "DE : 0x" << std::hex << std::setw(4) << std::setfill('0') << cpu->de << std::endl
               << "SP : 0x" << std::hex << std::setw(4) << std::setfill('0') << cpu->sp;

            text->SetLabelText(wxString(ss.str().c_str()));
        }
    }

    cpu_state_t *cpu;
    wxTimer timer;
    wxStaticText *text;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_TIMER(0, MainFrame::ShowFPS)
wxEND_EVENT_TABLE()

#endif
