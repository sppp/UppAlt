#ifndef _SDL2GUI3DAlt_SDL2GUI3DAlt_h_
#define _SDL2GUI3DAlt_SDL2GUI3DAlt_h_

#include <OOSDL2/OOSDL2.h>


NAMESPACE_UPP

//struct SDL2GUI3DAlt_MachineData;
class SDL2GUI3DAlt;



class SDL2GUI3DAlt :
	public Sppp::OOSDL2::Bundle,
	VirtualGui3DAlt,
	VirtualSound
{
	
	
protected:
	
	//SDL2GUI3DAlt_MachineData* data = 0;
	
	Sppp::OOSDL2::AudioOutput* aout = 0;
	Sppp::OOSDL2::Screen* scr = 0;
	Sppp::OOSDL2::Events* ev = 0;
	
	SystemSound					empty_snd;
	SystemDraw					empty_draw;
	String						desired_title;
	
	
public:
	bool            Poll(Upp::CtrlEvent& e) override {return ev ? ev->Poll(e) : false;}
	Size            GetSize() override {return scr ? scr->GetSize() : Size(0,0);}
	void            SetTitle(String title) override {desired_title = title; if (scr) scr->SetTitle(title);}
	SystemDraw&     BeginDraw() override {return scr ? scr->BeginDraw() : empty_draw;}
	void            CommitDraw() override {if (scr) scr->CommitDraw();}
	uint32          GetTickCount() override {return ev ? ev->GetTickCount() : 0;}
	void            WaitEvent(int ms) override {if (ev) ev->WaitEvent(ms);}
	bool            IsWaitingEvent() override {return ev ? ev->IsWaitingEvent() : false;}
	void            WakeUpGuiThread() override {if (ev) ev->WakeUpEventThread();}
	
	/*dword           GetMouseButtons() override;
	dword           GetModKeys() override;
	bool            IsMouseIn() override;
	void            SetMouseCursor(const Image& image) override;
	void            Quit() override {Close();}*/

	
	SystemSound&	BeginPlay() override {return aout ? aout->GetSystemSound() : empty_snd;}
	void			CommitPlay() override;
	int				GetAudioSampleRate() override {return aout ? aout->GetSampleRate() : 0;}
	int				GetAudioChannels() override {return aout ? aout->GetChannels() : 0;}
	int				GetAudioFrequency() override {return aout ? aout->GetFrequency() : 0;}
	int				GetAudioSampleSize() override {return aout ? aout->GetSampleSize() : 0;}
	bool			IsAudioSampleFloating() override {return aout ? aout->IsSampleFloating() : 0;}
	void			UndoPlay() override;
	
	
	bool			Open();
	void			Close();
	//bool			IsOpen() const {return is_open;}
	//SDL2GUI3DAlt_MachineData* GetData() {return data;}
	void			RecvAudio(Uint8* stream, int len);
	
	
	bool			InitMachine();
	bool			DeinitMachine();
	//void			Render(bool do_render);
	//void			RenderFrame();
	//void			RenderCamera();
	//void			RenderWindows();
	
	SDL2GUI3DAlt();
	~SDL2GUI3DAlt();
	
	static SDL2GUI3DAlt* Current();
	
};



END_UPP_NAMESPACE


#define GUI_APP_MAIN_(init_ecs) \
void GuiMainFn_(); \
\
extern "C" int main(int argc, char *argv[]) {\
	::UPP::AppInit__(argc, (const char **)argv, (const char**)environ); \
	Sppp::SDL2GUI3DAlt gui; \
	if (gui.Open()) { \
		if (init_ecs) {\
			if (gui.InitMachine()) { \
				GuiMainFn_(); \
				gui.DeinitMachine(); \
			} \
		} \
		else { \
			GuiMainFn_(); \
		} \
		gui.Close(); \
	} \
	return ::UPP::AppExit__(); \
} \
\
void GuiMainFn_()


#define GUI_APP_MAIN			GUI_APP_MAIN_(true)
#define RENDER_APP_MAIN			GUI_APP_MAIN_(false)


#endif
