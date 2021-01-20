#ifndef _SDL2GUIAlt_SDL2GUIAlt_h_
#define _SDL2GUIAlt_SDL2GUIAlt_h_

#include <CtrlLibAlt/CtrlLibAlt.h>
#include <GLDrawAlt/GLDrawAlt.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


NAMESPACE_UPP


struct SDL2GUI3D : VirtualGui3DAlt {
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

	SDL_Window   *win;
	SDL_GLContext glcontext;
	int64         serial;
	GLDraw        gldraw;
	SystemDraw    sysdraw;

	void Attach(SDL_Window *win, SDL_GLContext glcontext);
	void Detach();

	bool Create(const Rect& rect, const char *title);
	void Destroy();
	
	SDL2GUI3D();
	~SDL2GUI3D();
};


END_UPP_NAMESPACE

#endif
