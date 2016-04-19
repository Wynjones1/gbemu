#include "platform.h"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !PLATFORM_SDL
#ifdef __cplusplus
extern "C" {
#endif
	void delay(uint32_t ms)
	{
		wxMilliSleep(ms);
	}

	uint32_t get_ticks(void)
	{
		static wxLongLong start;
		if (start == 0)
		{
			start = wxGetUTCTimeMillis();
		}
		return (uint32_t)(wxGetUTCTimeMillis() - start).GetValue();
	}

#ifdef __cplusplus
} // extern "C"
#endif
#endif