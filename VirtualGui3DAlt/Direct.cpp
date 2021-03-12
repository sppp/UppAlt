#include <EcsLib/EcsLib.h>


NAMESPACE_SPPP_BEGIN

uint32 DirectWindow::prev_ticks;


DirectWindow::DirectWindow() {
	SetFPS(60);
	SetAudioSyncInterval(1.5);
	
}

void DirectWindow::SetTitle(String title) {
	VirtualGui3DAltPtr->SetTitle(title);
}

void DirectWindow::Initialize() {
	uint32 flags = 0;
	
	Shared<EventSystem> ev_sys = GetEntity().GetMachine().Get<EventSystem>();
	if (!ev_sys)
		return;
	
	prev_ticks = VirtualGui3DAltPtr->GetTickCount();
	
	ev_sys		-> AddControllable(*this);
	
	config.dt = 0;
	aconfig.sample_pos = 0;
	
	is_open = true;
}

void DirectWindow::RecvScreenSink(ScreenSource& src, float dt) {
	frame_age += dt;
	config.dt += dt;
	
	if (frame_age >= config.fps_dt) {
		if (frame_age > 2 * config.fps_dt)
			frame_age = config.fps_dt;
		else
			frame_age = Modulus(frame_age, config.fps_dt);
		
		
		SystemDraw& draw = VirtualGui3DAltPtr->BeginDraw();
		
		src.Render(config, draw);
		
		VirtualGui3DAltPtr->CommitDraw();
		
	}
}

void DirectWindow::RecvAudioSink(AudioSource& src, float dt) {
	av_sync_age += dt;
	aconfig.dt += dt;
	
	SystemSound& snd = VirtualSoundPtr->BeginPlay();
	
	if (av_sync_age >= aconfig.sync_dt) {
		if (av_sync_age > 2 * aconfig.sync_dt)
			av_sync_age = aconfig.sync_dt;
		else
			av_sync_age = Modulus(av_sync_age, aconfig.sync_dt);
		
		aconfig.sample_pos = 0;
		aconfig.sync = true;
		
		src.Play(aconfig, snd);
		
		VirtualSoundPtr->CommitPlay();
	}
	else if (!snd.IsQueueFull()) {
		aconfig.sync = false;
		src.Play(aconfig, snd);
	}
	else {
		VirtualSoundPtr->UndoPlay();
	}
}

void DirectWindow::Uninitialize() {
	if (is_open)
		WhenClose();
	
	/*if (display_renderer) {
		SDL_DestroyRenderer(display_renderer);
		display_renderer = 0;
	}
	
	if (display_window) {
		SDL_DestroyWindow(display_window);
		display_window = 0;
	}
    
    if (fb) {
        SDL_DestroyTexture(fb);
        fb = 0;
    }*/
    
	
	Shared<EventSystem> ev_sys = GetEntity().GetMachine().Get<EventSystem>();
	if (ev_sys)
		ev_sys->RemoveControllable(*this);
	
	is_open = false;
}

void DirectWindow::EmitCtrlEvent(const CtrlEvent& e) {
	for(ControllerSink* sink : ControllerSource::GetSinks())
		sink->RecvCtrlEvent(e);
}


NAMESPACE_SPPP_END
