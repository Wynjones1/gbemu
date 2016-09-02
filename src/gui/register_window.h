#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H
#include <wx/wx.h>
#include <wx/grid.h>
#include "cpu.h"

std::string format_hex(uint32_t value, uint8_t bytes=2)
{
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0') << std::setw(bytes * 2) << value;
    return ss.str();
}

class RegisterWindow : public wxGrid
{
public:
    RegisterWindow(wxWindow *parent, cpu_state_t *cpu_)
    : wxGrid(parent, 0, wxPoint(0, 0), wxSize(300, 300))
    , render_timer(this, 0)
    , cpu(cpu_)
    {
        CreateGrid(6, 2);
        HideRowLabels();
        
        SetColLabelValue(0, "Register");
        SetColLabelValue(1, "Value");

        render_timer.Start(100);
        SetDefaultCellFont(wxFontInfo(12).Bold().FaceName("Consolas"));
    }

    void RenderRegister(int row, const std::string &name, uint16_t value)
    {
        SetCellValue(wxGridCellCoords(row, 0), wxString(name));
        SetCellValue(wxGridCellCoords(row, 1), wxString(format_hex(value)));
    }

    void Render(wxTimerEvent &ev)
    {
        RenderRegister(0, "AF", cpu->af);
        RenderRegister(1, "BC", cpu->bc);
        RenderRegister(2, "DE", cpu->de);
        RenderRegister(3, "HL", cpu->hl);
        RenderRegister(4, "SP", cpu->sp);
        RenderRegister(5, "PC", cpu->pc);
    }

    wxTimer render_timer;

    cpu_state_t *cpu;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(RegisterWindow, wxGrid)
    EVT_TIMER(0, RegisterWindow::Render)
wxEND_EVENT_TABLE()

#endif