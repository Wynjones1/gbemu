#ifndef GUI_FRAME_H
#define GUI_FRAME_H

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
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

        auto sizer = new wxBoxSizer(wxHORIZONTAL);

        auto display = new PixelDisplay(this);
        display->Show(true);
        sizer->Add(display, 1, wxEXPAND);

        SetSizer(sizer);
        SetAutoLayout(true);
    }

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
wxEND_EVENT_TABLE()

#endif
