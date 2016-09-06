#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H
#include <wx/wx.h>
#include <wx/grid.h>

extern "C" {
    #include "instruction_strings.h"
    #include "opcodes.h"
}
//TODO: Remove this include once format_hex is somewhere more logical.
#include "register_window.h"

class DisassemblyWindow : public wxGrid
{
public:
    DisassemblyWindow(wxWindow *parent, cpu_state_t *cpu_)
    : wxGrid(parent, wxID_ANY)
    , render_timer(this, 0)
    , num_rows(25)
    , cpu(cpu_)
    {
        CreateGrid(num_rows, 5);
        HideRowLabels();
        SetColLabelValue(0, "address");
        SetColLabelValue(1, "opcode");
        SetColLabelValue(2, "instruction");
        SetColLabelValue(3, "arg 0");

        render_timer.Start(100);

        SetDefaultCellFont(wxFontInfo(12).Bold().FaceName("Consolas"));
    }

    void Render(wxTimerEvent &ev)
    {
        uint16_t offset = cpu->pc;
        bool last_cb=false;

        for (auto r = 0u; r < num_rows; r++)
        {
            auto address = format_hex(offset);
            auto x       = memory_load8(cpu->memory, offset);
            auto opcode  = format_hex(x);
            SetCellValue(wxGridCellCoords(r, 0), wxString(address));
            SetCellValue(wxGridCellCoords(r, 1), opcode);
            if (last_cb)
            {
                SetCellValue(wxGridCellCoords(r, 2), instruction_strings_cb[x]);
            }
            else
            {
                SetCellValue(wxGridCellCoords(r, 2), instruction_strings[x]);
            }

            last_cb = (!last_cb && x == 0xcb);
            offset += (last_cb ? 1 : op_table[x].size);
        }
    }

    wxTimer render_timer;
    uint32_t num_rows;
    cpu_state_t *cpu;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(DisassemblyWindow, wxGrid)
    EVT_TIMER(0, DisassemblyWindow::Render)
wxEND_EVENT_TABLE()
#endif
