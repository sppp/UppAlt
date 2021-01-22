#include "SDL2GUI3DAlt.h"

NAMESPACE_UPP

SDL2GUI3DAlt::SDL2GUI3DAlt() {
	
}

SDL2GUI3DAlt::~SDL2GUI3DAlt() {
	
}

bool SDL2GUI3DAlt::Create(const Rect& rect, const char *title) {
	
	// SDL
	uint32 sdl_flags =	SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
						SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR;
	if (SDL_Init(sdl_flags) < 0) {
		LOG("Error : " << SDL_GetError());
		return false;
	}
	
	
	// Window
	int w = rect.Width();
	int h = rect.Height();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	uint32 flags = SDL_WINDOW_OPENGL;
	//if (full_screen)
	//	flags |= SDL_WINDOW_FULLSCREEN;
	if (is_sizeable)
		flags |= SDL_WINDOW_RESIZABLE;
	if (is_maximized)
		flags |= SDL_WINDOW_MAXIMIZED;
	if (SDL_CreateWindowAndRenderer(w, h, flags, &win, &rend) == -1)
        return false;
    
    
    // Renderer
    SDL_GetRendererInfo(rend, &rend_info);
	if ((rend_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (rend_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        return false;
	}
	
	
	// GL context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	glcontext = SDL_GL_CreateContext(win);
	
	
	// Glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		LOG("Glew error: " << (const char*)glewGetErrorString(err));
		return false;
	}
	
	
	// Fonts
    TTF_Init();
    
    
    // Fullscreen
	#if defined flagDX12_PBR || defined flagOPENGL_PBR
	if (full_screen)
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
	#endif
	
    return true;
}

void SDL2GUI3DAlt::Quit() {
	GetDrawCommandCache().Clear();
    TTF_Quit();
    SDL_Quit();
    is_open = false;
}

Size SDL2GUI3DAlt::GetSize() {
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	return Size(w, h);
}

void SDL2GUI3DAlt::Maximize(bool b) {
	if (is_open) {
		/*SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		SDL_GetWMInfo(&info);
		ShowWindow(info.window, SW_MAXIMIZE);*/
	}
	is_maximized = true;
}


dword       SDL2GUI3DAlt::GetMouseButtons() {TODO}
dword       SDL2GUI3DAlt::GetModKeys() {TODO}
bool        SDL2GUI3DAlt::IsMouseIn() {TODO}
bool        SDL2GUI3DAlt::ProcessEvent(bool *quit) {TODO}
void        SDL2GUI3DAlt::WaitEvent(int ms) {TODO}
bool        SDL2GUI3DAlt::IsWaitingEvent() {TODO}
void        SDL2GUI3DAlt::WakeUpGuiThread() {TODO}
void        SDL2GUI3DAlt::SetMouseCursor(const Image& image) {TODO}
SystemDraw& SDL2GUI3DAlt::BeginDraw() {TODO}
void        SDL2GUI3DAlt::CommitDraw() {TODO}




void Ctrl::InitTimer() {
	SetGlobalTicks(0);
}

END_UPP_NAMESPACE
