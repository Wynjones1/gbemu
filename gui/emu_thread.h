#ifndef GUI_EMU_THREAD
#define GUI_EMU_THREAD

class EmuThread : public wxThread
{
public:
    EmuThread()
        : wxThread(wxTHREAD_DETACHED)
    {
    }
};
#endif