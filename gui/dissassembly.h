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
    : wxGrid(parent, 0, wxPoint(0, 0), wxSize(400, 300))
    , render_timer(this, 0)
    , num_rows(25)
    , cpu(cpu_)
    {
        CreateGrid(num_rows, 5);
        HideRowLabels();
        SetColLabelValue(0, "break");
        SetColLabelValue(1, "address");
        SetColLabelValue(2, "opcode");
        SetColLabelValue(3, "instruction");
        SetColLabelValue(4, "arg 0");

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
            auto opcode  = format_hex(x, 1);
            SetCellValue(wxGridCellCoords(r, 1), wxString(address));
            SetCellValue(wxGridCellCoords(r, 2), opcode);
            if (last_cb)
            {
                SetCellValue(wxGridCellCoords(r, 3), instruction_strings_cb[x]);
            }
            else
            {
                SetCellValue(wxGridCellCoords(r, 3), instruction_strings[x]);
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