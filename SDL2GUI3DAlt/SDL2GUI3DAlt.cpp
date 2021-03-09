#include <EcsLib/EcsLib.h>
#include "After.h"

NAMESPACE_UPP

bool __enable_opengl_debug;

void GLAPIENTRY
OpenGLMessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
	if (!__enable_opengl_debug)
		return;
	String s;
	s << "OpenGL debug: ";
	if (type == GL_DEBUG_TYPE_ERROR)
		s << "error: ";
	s << "type = " << HexStr(type);
	s << ", severity = " << HexStr(severity);
	s << ", message = " << String(message);
	LOG(s);
}

void EnableOpenGLDebugMessages(bool b) {__enable_opengl_debug = b;}





SDL2GUI3DAlt_MachineData __sdl2data;
SDL2GUI3DAlt* __current_SDL2GUI3DAlt;


SDL2GUI3DAlt::SDL2GUI3DAlt() {
	__current_SDL2GUI3DAlt = this;
	data = &__sdl2data;
}

SDL2GUI3DAlt::~SDL2GUI3DAlt() {
	if (is_open)
		Quit();
}

SDL2GUI3DAlt* SDL2GUI3DAlt::Current() {
	return __current_SDL2GUI3DAlt;
}

bool SDL2GUI3DAlt::InitMachine() {
	Machine& mach = GetMachine();
	
	try {
	    mach.Add<RegistrySystem>();
	    mach.Add<EventSystem>();
	    mach.Add<RenderingSystem>();
	    mach.Add<WindowSystem>();
	    #ifdef flagOPENVR
	    mach.Add<OpenVR>();
	    #endif
	    
	    EntityStore& ents = *mach.Add<EntityStore>();
	    mach.Add<ComponentStore>();
	    mach.Add<HolographicScene>();
	    mach.Add<EasingSystem>();
	    mach.Add<AudioSystem>();
	    mach.Add<ControllerSystem>();
	    mach.Add<MotionControllerSystem>();
	    mach.Add<WorldLogicSystem>();
	    
	    /*bool physics = false;
	    if (physics)
			mach.Add<PhysicsSystem>();*/
	
	    mach.Start();
	    
		ents.Create<CameraPrefab>();
	}
	catch (Exc e) {
		LOG("InitMachine error: " << e);
		return false;
	}
	
	return true;
}

bool SDL2GUI3DAlt::DeinitMachine() {
	GetMachine().SetNotRunning();
	
	return true;
}

bool SDL2GUI3DAlt::Create(const Rect& rect, const char *title, bool init_ecs) {
	AppFlags& app_flags = GetAppFlags();
	ASSERT(!VirtualGui3DAltPtr);
	VirtualGui3DAltPtr = this;
	VirtualSoundPtr = this;
	
	// SDL
	uint32 sdl_flags =	SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
						SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR;
	if (SDL_Init(sdl_flags) < 0) {
		LOG("Error : " << SDL_GetError());
		return false;
	}
	
	
	// Window
	screen_sz = rect.GetSize();
	uint32 flags = 0;
	
	if (app_flags.IsOpenGL()) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		flags |= SDL_WINDOW_OPENGL;
	}
	//if (full_screen)
	//	flags |= SDL_WINDOW_FULLSCREEN;
	if (is_sizeable)
		flags |= SDL_WINDOW_RESIZABLE;
	if (is_maximized)
		flags |= SDL_WINDOW_MAXIMIZED;
	if (SDL_CreateWindowAndRenderer(screen_sz.cx, screen_sz.cy, flags, &win, &rend) == -1)
        return false;
	SDL_SetWindowTitle(win, title);
    
    // Renderer
    SDL_GetRendererInfo(rend, &rend_info);
	if ((rend_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (rend_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        return false;
	}
	
	
	// GL context
	if (app_flags.IsOpenGL()) {
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
		
		#ifdef flagDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback( OpenGLMessageCallback, 0 );
		#endif
	}
	
	
	// Software framebuffer
	if (app_flags.IsSoftwareRenderer()) {
		int fb_stride = 3;
		SDL_Texture* fb = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, screen_sz.cx, screen_sz.cy);
		if (!fb) {
			LOG("error: couldn't create framebuffer texture");
		}
		
		data->sw_rend.GetOutputSoftFramebuffer().Init(fb, screen_sz.cx, screen_sz.cy, fb_stride);
	}
	
	// Images
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	
	// Fonts
    TTF_Init();
    
    
    // Fullscreen
	#if defined flagDX12_PBR || defined flagOPENGL_PBR
	if (full_screen)
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
	#endif
	
	
	// Init ECS machine
	if (init_ecs && !InitMachine())
		return false;
	
	is_open = true;
    return true;
}

void SDL2GUI3DAlt::Quit() {
	GetDrawCommandCache().Clear();
	DeinitMachine();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    is_open = false;
}

SystemSound& SDL2GUI3DAlt::BeginPlay() {
	syssnd.Set(&native_snd);
	
	return syssnd;
}

void SDL2GUI3DAlt::CommitPlay() {
	
	TODO
	
}

void SDL2GUI3DAlt::UndoPlay() {
	
	TODO
	
}

int SDL2GUI3DAlt::GetSampleRate() {
	
	TODO
	
}

void SDL2GUI3DAlt_Sound::Put(float* v, int size, bool realtime) {
	
	TODO
	
}

int SDL2GUI3DAlt_Sound::GetQueueSize() const {
	
	TODO
	
}

int SDL2GUI3DAlt_Sound::GetSampleRate() const {
	
	TODO
	
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

uint32 SDL2GUI3DAlt::GetTickCount() {
	return SDL_GetTicks();
}

/*bool SDL2GUI3DAlt::ProcessEvent(bool *quit) {
	CtrlEvent e;
	Poll(e);
	if (e.type == EVENT_SHUTDOWN)
		*quit = true;
	return false;
}*/

void SDL2GUI3DAlt::PutKeyFlags() {
	if (is_lalt || is_ralt)			key |= K_ALT;
	if (is_lshift || is_rshift)		key |= K_SHIFT;
	if (is_lctrl || is_rctrl)		key |= K_CTRL;
}

bool SDL2GUI3DAlt::Poll(Upp::CtrlEvent& e) {
	SDL_Event event;
	
	// Process the events
	while (SDL_PollEvent(&event)) {
		e.Clear();
		
	switch (event.type) {
		
		case SDL_WINDOWEVENT:
			
			/*if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
				if (IsCaptured())
					;//Ctrl::captured->LeftUp(prev_mouse_pt, 0);
				else
					DeepMouseLeave();
			}
			else*/
			if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
				e.type = EVENT_SHUTDOWN;
				return true;
			}
			else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				screen_sz.cx = event.window.data1;
				screen_sz.cy = event.window.data2;
				/*SetFrameRect0(RectC(0, 0, screen_sz.cx, screen_sz.cy));
				SetContentRect(RectC(0, 0, screen_sz.cx, screen_sz.cyh));
				SetPendingLayout();
				SetPendingEffectRedraw();
				SetPendingRedraw();*/
				e.type = EVENT_WINDOW_RESIZE;
				e.sz = screen_sz;
				return true;
			}
			break;
		
			
		case SDL_KEYDOWN:
		
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:	event.type = SDL_QUIT; break;
				case SDLK_LALT:		is_lalt = true; break;
				case SDLK_RALT:		is_ralt = true; break;
				case SDLK_LSHIFT:	is_lshift = true; break;
				case SDLK_RSHIFT:	is_rshift = true; break;
				case SDLK_LCTRL:	is_lctrl = true; break;
				case SDLK_RCTRL:	is_rctrl = true; break;
			}
			
			key = event.key.keysym.sym;
			if (key & SDLK_SCANCODE_MASK) {
				key &= ~SDLK_SCANCODE_MASK;
				
				// TODO handle codes separately
				if (0 /*key == */) {
					
				}
				else key = 0;
			}
			PutKeyFlags();
			
			e.type = EVENT_KEYDOWN;
			e.value = key;
			e.n = 1;
			e.pt = Point(0,0);
			
			return true;
			
		case SDL_KEYUP:
		
			switch (event.key.keysym.sym) {
				case SDLK_LALT:		is_lalt = false; break;
				case SDLK_RALT:		is_ralt = false; break;
				case SDLK_LSHIFT:	is_lshift = false; break;
				case SDLK_RSHIFT:	is_rshift = false; break;
				case SDLK_LCTRL:	is_lctrl = false; break;
				case SDLK_RCTRL:	is_rctrl = false; break;
			}
			
			key = event.key.keysym.sym | K_KEYUP;
			if (key & SDLK_SCANCODE_MASK) {
				key &= ~SDLK_SCANCODE_MASK;
				
				// TODO handle codes separately
				if (0 /*key == */) {
					
				}
				else key = 0;
			}
			PutKeyFlags();
			
			e.type = EVENT_KEYUP;
			e.value = key;
			e.n = 1;
			e.pt = Point(0,0);
			
			return true;
			
		case SDL_MOUSEMOTION:
			mouse_pt = Point(event.motion.x, event.motion.y);
			key = 0;
			PutKeyFlags();
			
			e.type = EVENT_MOUSEMOVE;
			e.value = key;
			e.pt = mouse_pt;
			
			prev_mouse_pt = mouse_pt;
			return true;
		
		case SDL_MOUSEWHEEL:
			key = 0;
			PutKeyFlags();
			
			e.type = EVENT_MOUSEWHEEL;
			e.value = key;
			e.pt = mouse_pt;
			
			return true;
			
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			
			mouse_code = 0;
			//mouse_zdelta = 0;
			if (event.button.state == SDL_PRESSED) {
				if (event.button.clicks == 1) {
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse_code = Ctrl::LEFT_DOWN;
					else if (event.button.button == SDL_BUTTON_MIDDLE)
						mouse_code = Ctrl::MIDDLE_DOWN;
					else if (event.button.button == SDL_BUTTON_RIGHT)
						mouse_code = Ctrl::RIGHT_DOWN;
				}
				else if (event.button.clicks == 2) {
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse_code = Ctrl::LEFT_DOUBLE;
					else if (event.button.button == SDL_BUTTON_MIDDLE)
						mouse_code = Ctrl::MIDDLE_DOUBLE;
					else if (event.button.button == SDL_BUTTON_RIGHT)
						mouse_code = Ctrl::RIGHT_DOUBLE;
				}
				else {
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse_code = Ctrl::LEFT_TRIPLE;
					else if (event.button.button == SDL_BUTTON_MIDDLE)
						mouse_code = Ctrl::MIDDLE_TRIPLE;
					else if (event.button.button == SDL_BUTTON_RIGHT)
						mouse_code = Ctrl::RIGHT_TRIPLE;
				}
				/*else if (event.button.button == SDL_BUTTON_WHEELUP)
					mouse_zdelta = 120;
				else if (event.button.button == SDL_BUTTON_WHEELDOWN)
					mouse_zdelta = -120;*/
			}
			else if (event.button.state == SDL_RELEASED) {
				if (event.button.button == SDL_BUTTON_LEFT)
					mouse_code = Ctrl::LEFT_UP;
				else if (event.button.button == SDL_BUTTON_MIDDLE)
					mouse_code = Ctrl::MIDDLE_UP;
				else if (event.button.button == SDL_BUTTON_RIGHT)
					mouse_code = Ctrl::RIGHT_UP;
			}
			
			if (mouse_code) {
				mouse_pt = Point(event.button.x, event.button.y);
				key = 0;
				PutKeyFlags();
				
				e.type = EVENT_MOUSE_EVENT;
				e.value = key;
				e.pt = mouse_pt;
				e.n = mouse_code;
				
				prev_mouse_pt = mouse_pt;
				return true;
			}
			
		default:
			break;
		}
	}
	
	
	return false;
}

void SDL2GUI3DAlt::WaitEvent(int ms) {
	SDL_WaitEventTimeout(NULL, ms);
}

bool SDL2GUI3DAlt::IsWaitingEvent() {
	SDL_PumpEvents();
	SDL_Event events;
	return SDL_PeepEvents(&events, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) > 0;
}

void SDL2GUI3DAlt::WakeUpGuiThread() {
	//waketimer_id = SDL_AddTimer(20, WakeCb, NULL);
}

SystemDraw& SDL2GUI3DAlt::BeginDraw() {
	AppFlags& flags = GetAppFlags();
	if (flags.IsSoftwareRenderer()) {
		auto& rend = data->sw_rend;
		auto& draw = data->sw_draw;
		
		rend.screen_sz = screen_sz;
	    rend.win = win;
	    rend.rend = this->rend;
	    rend.PreFrame();
	    draw.rend = &rend;
	    draw.fb = &rend.GetOutputSoftFramebuffer();
	    sysdraw.ptr = &draw;
	    
	    data->sw_draw.fb->Enter();
	}
	else if (flags.IsOpenGL()) {
		auto& rend = data->hw_rend;
		auto& draw = data->hw_draw;
		
		rend.screen_sz = screen_sz;
	    rend.win = win;
	    rend.rend = this->rend;
	    rend.PreFrame();
	    draw.rend = &rend;
	    draw.fb = &rend.GetOutputHardFramebuffer();
	    sysdraw.ptr = &draw;
	    
	    data->hw_draw.fb->Enter();
	}
	else {
		TODO
	}
	
	return sysdraw;
}

dword SDL2GUI3DAlt::GetMouseButtons() {TODO}
dword SDL2GUI3DAlt::GetModKeys() {TODO}
bool  SDL2GUI3DAlt::IsMouseIn() {TODO}
void  SDL2GUI3DAlt::SetMouseCursor(const Image& image) {TODO}

void  SDL2GUI3DAlt::CommitDraw() {
	AppFlags& flags = GetAppFlags();
	if (flags.IsSoftwareRenderer()) {
		data->sw_draw.fb->Leave();
		data->sw_rend.PostFrame();
	}
	else if (flags.IsOpenGL()) {
		data->hw_draw.fb->Leave();
		data->hw_rend.PostFrame();
	}
}




END_UPP_NAMESPACE
