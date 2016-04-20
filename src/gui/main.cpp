// For compilers that support precompilation, includes "wx.h".
#include <sstream>
#include <iomanip>
#include "wx/glcanvas.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
#error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../../sample.xpm"
#endif

#include "cpu.h"

enum
{
	REFRESH_TIMER_ID = 10,
	UI_TIMER_ID,
	REGISTER_TIMER_ID,
};

class MainWindow : public wxGLCanvas
{
public:
	template<typename... Args>
	MainWindow(cpu_state_t *cpu_, Args... args)
		: wxGLCanvas(std::forward<Args>(args)...)
		, cpu(cpu_)
	{
		gl_context = new wxGLContext(this);
		timer = new wxTimer(this, REFRESH_TIMER_ID);
		timer->Start(17);

		SetBackgroundStyle(wxBG_STYLE_CUSTOM);
		SetMinSize(wxSize(3 * LCD_WIDTH, 3 * LCD_HEIGHT));
		Fit();
	}


	void render(wxPaintEvent &evt)
	{
		wxGLCanvas::SetCurrent(*gl_context);
		wxPaintDC(this);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static GLuint texture_id = 0;
		if (texture_id == 0)
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &texture_id);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LCD_WIDTH, LCD_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, display_get_framebuffer(cpu->display));

		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(0, 0); glVertex2f(-1.0f, 1.0f);
			glTexCoord2f(1, 0); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(1, 1); glVertex2f(1.0f, -1.0f);
		glEnd();

		glFlush();
		SwapBuffers();
	}


#define X(new, old) do{\
					mem->dpad.new = 0;\
					if(mem->dpad.old == 0)\
						mem->dpad.old = 1;\
					}while(0);

	void handle_events()
	{
		memory_t *mem = cpu->memory;
		static int paused;
		static int step;
		static int frame_limit;
		static int slow;

		mem->dpad.up    = 1;
		mem->dpad.down  = 1;
		mem->dpad.left  = 1;
		mem->dpad.right = 1;

		if (wxGetKeyState(wxKeyCode('W'))) { X(up, down);}
		if (wxGetKeyState(wxKeyCode('A'))) { X(left, right); }
		if (wxGetKeyState(wxKeyCode('S'))) { X(down, up); }
		if (wxGetKeyState(wxKeyCode('D'))) { X(right, left); }
		if (wxGetKeyState(wxKeyCode('W'))) { X(up, down); }

		if (wxGetKeyState(wxKeyCode('P')))
		{
			cpu->paused = !cpu->paused;
		}

		if (wxGetKeyState(wxKeyCode('Q')))
		{
			cpu->frame_limit = !cpu->frame_limit;
		}

		mem->buttons.a      = !wxGetKeyState(wxKeyCode('I'));
		mem->buttons.b      = !wxGetKeyState(wxKeyCode('J'));
		mem->buttons.start  = !wxGetKeyState(wxKeyCode('F'));
		mem->buttons.select = !wxGetKeyState(wxKeyCode('G'));

		SET_N(mem->IF, JOYPAD_BIT);
	}
#undef X

	void resized(wxSizeEvent &evt)
	{
		Refresh();
	}

	void timer_expired(wxTimerEvent &evt)
	{
		Refresh();
		handle_events();
	}

	DECLARE_EVENT_TABLE();

private:
	wxGLContext *gl_context;
	cpu_state_t *cpu;
	wxTimer *timer;
};

BEGIN_EVENT_TABLE(MainWindow, wxGLCanvas)
	EVT_PAINT(MainWindow::render)
	EVT_SIZE(MainWindow::resized)
	EVT_TIMER(REFRESH_TIMER_ID, MainWindow::timer_expired)
END_EVENT_TABLE()

class RegisterInfo : public wxPanel
{
public:
	template<typename... Args>
	RegisterInfo(wxString &label_, uint16_t *data_, Args... args)
		: wxPanel(std::forward<Args>(args)...)
		, data(data_)
	{
		sizer = new wxBoxSizer(wxHORIZONTAL);
		label = new wxStaticText(this, -1, label_);
		text = new wxTextCtrl(this, -1);
		timer = new wxTimer(this, REGISTER_TIMER_ID);

		label->SetMinSize(wxSize(25, 20));
		label->SetBackgroundColour(wxColour(0xff, 0xff, 0xff));
		text->SetMinSize(wxSize(50, 20));

		timer->Start(100);
		sizer->Add(label);
		sizer->Add(text);
		SetSizer(sizer);
		Fit();
	}

	void UpdateUI(wxTimerEvent &ev)
	{
		char buf[1024];
		sprintf(buf, "0x%04X", *data);
		text->SetLabelText(buf);
	}

	wxBoxSizer   *sizer;
	wxStaticText *label;
	wxTextCtrl   *text;
	wxTimer *timer;
	uint16_t *data;

	DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(RegisterInfo, wxPanel)
	EVT_TIMER(REGISTER_TIMER_ID, RegisterInfo::UpdateUI)
END_EVENT_TABLE()

class DebugWindow : public wxPanel
{
public:

	void AddRegister(const char *str, uint16_t *data)
	{
		registers.push_back(new RegisterInfo(wxString(str), data, this, -1));
		sizer->Add(registers.back());
	}

	template<typename... Args>
	DebugWindow(cpu_state_t *cpu_, Args... args)
		: wxPanel(std::forward<Args>(args)...)
		, cpu(cpu_)
	{
		SetBackgroundColour(wxColour(0x00, 0xff, 0x00));

		sizer = new wxBoxSizer(wxVERTICAL);

		AddRegister("PC", &cpu->pc);
		AddRegister("AF", &cpu->af);
		AddRegister("BC", &cpu->bc);
		AddRegister("DE", &cpu->de);
		AddRegister("HL", &cpu->hl);
		AddRegister("SP", &cpu->sp);
		SetSizer(sizer);
		Fit();

	}

	cpu_state_t *cpu;
	std::vector<RegisterInfo*> registers;
	wxBoxSizer *sizer;
};


class CPUThread : public wxThread
{
public:
	CPUThread(cpu_state_t *state_)
		: wxThread(wxTHREAD_DETACHED)
		, state(state_)
	{}

	virtual ExitCode Entry()
	{
		cpu_start(state);
		return 0;
	}
private:
	cpu_state_t *state;
};

class MainFrame : public wxFrame
{
public:
	template<typename... Args>
	MainFrame(Args... args)
		: wxFrame(std::forward<Args>(args)...)
		, cpu(cpu_init())
	{
		auto sizer = new wxBoxSizer(wxHORIZONTAL);

		auto timer = new wxTimer(this, UI_TIMER_ID);
		timer->Start(100);

		auto menu_bar = new wxMenuBar();
		auto menu     = new wxMenu();
		menu->Append(wxID_OPEN, wxString("&Open"));
		menu->Append(wxID_EXIT, wxString("&Quit"));
		menu_bar->Append(menu, "File");
		SetMenuBar(menu_bar);

		status_bar = new wxStatusBar(this);
		status_bar->SetFieldsCount(3);
		status_bar->PushStatusText("100.0%"          , 0);
		status_bar->PushStatusText("Paused : No",  1);
		status_bar->PushStatusText("Frame Limit : Yes", 2);
		SetStatusBar(status_bar);

		auto main_window = new MainWindow(cpu, this, -1);
		sizer->Add(main_window);
#if 1
		auto debug_window = new DebugWindow(cpu, this, -1);
		sizer->Add(debug_window);
#endif
		SetSizer(sizer);
		Fit();
		SetMinSize(GetSize());
		SetMaxSize(GetSize());

		cpu_thread = new CPUThread(cpu);
		cpu_thread->Run();
	}

	template<typename... Args>
	void PrintStatus(int idx, const char *format, Args... args)
	{
		char buf[1024];
		sprintf(buf, format, args...);
		status_bar->PopStatusText(idx);
		status_bar->PushStatusText(wxString(buf), idx);
	}
	void UpdateUI(wxTimerEvent &ev)
	{
		PrintStatus(0, "%4.2f%%", cpu->fps);
		PrintStatus(1, "Paused: %s", cpu->paused ? "Yes" : "No");
		PrintStatus(2, "Frame Limit: %s", cpu->frame_limit ? "Yes" : "No");
	}

	void OpenROM(wxCommandEvent &evt)
	{
		auto dialog = new wxFileDialog(this, wxString("Choose a ROM."),
			wxEmptyString, wxEmptyString,
			wxString("Rom files (*.gb)|*.gb"), wxFD_OPEN, wxDefaultPosition);
		if (dialog->ShowModal() == wxID_OK)
		{
			cmdline_args.in = dialog->GetPath();
			// Load the rom.
		}
		dialog->Destroy();
	}

	DECLARE_EVENT_TABLE();

private:
	cpu_state_t *cpu;
	CPUThread   *cpu_thread;

	wxStatusBar *status_bar;
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_TIMER(UI_TIMER_ID, MainFrame::UpdateUI)
	EVT_MENU(wxID_OPEN, MainFrame::OpenROM)
END_EVENT_TABLE()


class GUI : public wxApp
{
public:
	GUI()
	{}

	virtual bool OnInit() override
	{
		cmdline_args.in = "C:/Users/stuar/Programming/gbemu/data/roms/mario.gb";
		auto frame = new MainFrame(nullptr, wxID_ANY, "wxGBemu", wxPoint(50, 50), wxSize(640, 480));
		frame->Show();
		return true;
	}
private:
};

IMPLEMENT_APP(GUI) 