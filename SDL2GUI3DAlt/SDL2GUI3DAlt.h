#ifndef _SDL2GUI3DAlt_SDL2GUI3DAlt_h_
#define _SDL2GUI3DAlt_SDL2GUI3DAlt_h_

#include <GL/glew.h>
#ifdef flagOPENVR
	#include <openvr.h>
#endif
#include <GL/gl.h>


#include <CtrlLibAlt/CtrlLib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


NAMESPACE_UPP


struct SDL2GUI3DAlt : VirtualGui3DAlt {
    SDL_Window* win = NULL;
    SDL_Renderer* rend = NULL;
    SDL_RendererInfo rend_info;
	SDL_GLContext glcontext;
	bool is_open = false;
	bool is_maximized = false;
	bool is_sizeable = false;
	
	
	virtual Size        GetSize();
	virtual dword       GetMouseButtons();
	virtual dword       GetModKeys();
	virtual bool        IsMouseIn();
	virtual bool        ProcessEvent(bool *quit);
	virtual void        WaitEvent(int ms);
	virtual bool        IsWaitingEvent();
	virtual void        WakeUpGuiThread();
	virtual void        SetMouseCursor(const Image& image);
	virtual SystemDraw& BeginDraw();
	virtual void        CommitDraw();

	virtual void        Quit();

	int64         serial;
	//GLDraw        gldraw;
	SystemDraw    sysdraw;

	void Attach(SDL_Window *win, SDL_GLContext glcontext);
	void Detach();

	bool Create(const Rect& rect, const char *title);
	void Destroy();
	
	void Maximize(bool b=true);
	
	SDL2GUI3DAlt();
	~SDL2GUI3DAlt();
};


END_UPP_NAMESPACE


#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
extern "C" int main(int argc, char *argv[]) {\
	UPP::AppInit__(argc, (const char **)argv, (const char**)environ); \
	SDL2GUI3DAlt gui; \
	if (gui.Create(Rect(0, 0, 1920, 1000), "Virtual Gui Test")) { \
		VirtualGui3DAltPtr = &gui; \
		Ctrl::InitFB(); \
		Ctrl::SetDesktopSize(gui.GetSize()); \
		GuiMainFn_(); \
		Ctrl::ExitFB(); \
		Ctrl::CloseTopCtrls(); \
	} \
	return ::Upp::AppExit__(); \
} \
\
void GuiMainFn_()


#endif
