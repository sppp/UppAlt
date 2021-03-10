#ifndef _SDL2GUI3DAlt_SDL2GUI3DAlt_h_
#define _SDL2GUI3DAlt_SDL2GUI3DAlt_h_


#include <CtrlLibAlt/CtrlLib.h>
#include <GL/GL.h>
#include <VirtualSound/VirtualSound.h>


NAMESPACE_UPP

struct SDL2GUI3DAlt_MachineData;
class SDL2GUI3DAlt;



class SDL2GUI3DAlt : VirtualGui3DAlt, VirtualSound {
	
	static const bool prefer_highend_audio = true;
	
protected:
    SDL_Window* win = NULL;
    SDL_Renderer* rend = NULL;
    SDL_RendererInfo rend_info;
	SDL_GLContext glcontext = 0;
	SDL_AudioSpec audio_fmt, audio_desired;
	SDL_AudioDeviceID audio_dev = 0;
	bool is_open = false;
	bool is_maximized = false;
	bool is_sizeable = false;
	bool mouse_captured = false;
	
	Size screen_sz;
	SDL2GUI3DAlt_MachineData* data = 0;
	
	bool is_lalt = false, is_lshift = false, is_lctrl = false;
	bool is_ralt = false, is_rshift = false, is_rctrl = false;
	uint32 key = 0;
	uint32 mouse_code;
	int mouse_zdelta, x, y, w, h, dx, dy;
	Point prev_mouse_pt, mouse_pt;
	
	int64						serial;
	SystemDraw					sysdraw;
	SystemSound					syssnd;
	Sppp::VolatileSoundBuffer	snd_buf;
	
	
	
public:
	Size            GetSize() override;
	dword           GetMouseButtons() override;
	dword           GetModKeys() override;
	bool            IsMouseIn() override;
	bool            Poll(Upp::CtrlEvent& e) override;
	void            WaitEvent(int ms) override;
	bool            IsWaitingEvent() override;
	void            WakeUpGuiThread() override;
	void            SetMouseCursor(const Image& image) override;
	SystemDraw&     BeginDraw() override;
	void            CommitDraw() override;
	uint32          GetTickCount() override;
	void            SetTitle(String title) override {SDL_SetWindowTitle(win, title);}

	void            Quit() override;
	
	SystemSound&	BeginPlay() override;
	void			CommitPlay() override;
	int				GetAudioSampleRate() override {return audio_fmt.samples;}
	int				GetAudioChannels() override {return audio_fmt.channels;}
	int				GetAudioFrequency() override {return audio_fmt.freq;}
	int				GetAudioSampleSize() override;
	bool			IsAudioSampleFloating() override;
	void			UndoPlay() override;
	
	
	bool			Create(const Rect& rect, const char *title, bool init_ecs);
	void			Destroy();
	void			PutKeyFlags();
	void			RecvAudio(Uint8* stream, int len);
	void			Maximize(bool b=true);
	SDL2GUI3DAlt&	Sizeable(bool b=true) {is_sizeable = b; return *this;}
	void			SetDesiredAudioFmt(int sample_freq, int sample_bytes, bool is_sample_floating, int channels, int sample_rate);
	bool			IsCaptured() const {return mouse_captured;}
	bool			IsOpen() const {return is_open;}
	SDL2GUI3DAlt_MachineData* GetData() {return data;}
	
	
	bool			InitMachine();
	bool			DeinitMachine();
	void			Render(bool do_render);
	void			RenderFrame();
	void			RenderCamera();
	void			RenderWindows();
	
	SDL2GUI3DAlt();
	~SDL2GUI3DAlt();
	
	static SDL2GUI3DAlt* Current();
	
};

void EnableOpenGLDebugMessages(bool b);

inline bool IsGuiOpen() {return SDL2GUI3DAlt::Current()->IsOpen();}

END_UPP_NAMESPACE


#define GUI_APP_MAIN_(init_ecs) \
void GuiMainFn_(); \
\
extern "C" int main(int argc, char *argv[]) {\
	::UPP::AppInit__(argc, (const char **)argv, (const char**)environ); \
	Sppp::SDL2GUI3DAlt gui; \
	gui.Sizeable(); \
	if (gui.Create(::UPP::Rect(0, 0, 1280, 720), "Unnamed App", init_ecs)) { \
		GuiMainFn_(); \
	} \
	return ::UPP::AppExit__(); \
} \
\
void GuiMainFn_()


#define GUI_APP_MAIN			GUI_APP_MAIN_(true)
#define RENDER_APP_MAIN			GUI_APP_MAIN_(false)


#endif
