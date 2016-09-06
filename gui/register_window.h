#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H
#include <wx/wx.h>
#include <wx/grid.h>
#include "cpu.h"

template<typename T>
std::string format_hex(T value)
{
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0') << std::setw(sizeof(T) * 2) << +value;
    return ss.str();
}

class RegisterWindow : public wxGrid
{
public:
    RegisterWindow(wxWindow *parent, cpu_state_t *cpu_)
    : wxGrid(parent, wxID_ANY)
    , render_timer(this, 0)
    , cpu(cpu_)
    {
        CreateGrid(30, 2);
        HideRowLabels();
        
        SetColLabelValue(0, "Register");
        SetColLabelValue(1, "Value");

        render_timer.Start(100);
        SetDefaultCellFont(wxFontInfo(12).Bold().FaceName("Consolas"));
    }

    template<typename T>
    void RenderRegister(int row, const std::string &name, T value)
    {
        SetCellValue(wxGridCellCoords(row, 0), wxString(name));
        SetCellValue(wxGridCellCoords(row, 1), wxString(format_hex(value)));
    }

    void Render(wxTimerEvent &ev)
    {
        int idx = 0;
        RenderRegister(idx++, "AF"    , cpu->af);
        RenderRegister(idx++, "BC"    , cpu->bc);
        RenderRegister(idx++, "DE"    , cpu->de);
        RenderRegister(idx++, "HL"    , cpu->hl);
        RenderRegister(idx++, "SP"    , cpu->sp);
        RenderRegister(idx++, "PC"    , cpu->pc);
        RenderRegister(idx++, "SCX"   , cpu->memory->scx);
        RenderRegister(idx++, "SCY"   , cpu->memory->scy);
        RenderRegister(idx++, "WX"    , cpu->memory->wx);
        RenderRegister(idx++, "WY"    , cpu->memory->wy);
        RenderRegister(idx++, "LY"    , cpu->memory->ly);
        RenderRegister(idx++, "LYC"   , cpu->memory->lyc);
        RenderRegister(idx++, "IF"    , cpu->memory->IF);
        RenderRegister(idx++, "IE"    , cpu->memory->IE);
        RenderRegister(idx++, "IME"   , cpu->memory->IME);
        RenderRegister(idx++, "DIV"   , cpu->memory->div);
        RenderRegister(idx++, "TIMA"  , cpu->memory->tima);
        RenderRegister(idx++, "TMA"   , cpu->memory->tma);
        RenderRegister(idx++, "TAC.cs", cpu->memory->tac.clock_select);
        RenderRegister(idx++, "TAC.en", cpu->memory->tac.enable);
        RenderRegister(idx++, "STAT.mode"    , cpu->memory->stat.mode);
        RenderRegister(idx++, "STAT.coin"    , cpu->memory->stat.coincidence);
        RenderRegister(idx++, "STAT.hblnk"   , cpu->memory->stat.h_blank_int);
        RenderRegister(idx++, "STAT.vblnk"   , cpu->memory->stat.v_blank_int);
        RenderRegister(idx++, "STAT.oam"     , cpu->memory->stat.oam_int);
        RenderRegister(idx++, "STAT.coin_int", cpu->memory->stat.coincidence_int);
        RenderRegister(idx++, "noise freq", noise_freq(cpu->memory->audio));
        RenderRegister(idx++, "noise clk", noise_clk_count(cpu->memory->audio));

        AutoSize();
    }

    wxTimer render_timer;

    cpu_state_t *cpu;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(RegisterWindow, wxGrid)
    EVT_TIMER(0, RegisterWindow::Render)
wxEND_EVENT_TABLE()

#endif
