#include "System.h"

NAMESPACE_SPPP_BEGIN


SDL2System::SDL2System(Machine& m) : System<SDL2System>(m) {
	
}

bool SDL2System::Initialize() {
	return true;
}

void SDL2System::Start() {
	
}

void SDL2System::Update(float dt) {
	
}

void SDL2System::Stop() {
	
}

void SDL2System::Uninitialize() {
	
}



#define OBJ_CREATE \
	auto* ev_ctx = GetEntity().Find<SDL2ContextComponent>(); \
	if (ev_ctx) { \
		obj.Create(ev_ctx->GetOOSDL2()); \
	}

void SDL2TimerComponent::Initialize() {
	OBJ_CREATE
}

void SDL2TimerComponent::Uninitialize() {
	obj.Clear();
}



void SDL2AudioInputComponent::Initialize() {
	OBJ_CREATE
}

void SDL2AudioInputComponent::Uninitialize() {
	obj.Clear();
}

void SDL2AudioInputComponent::EmitAudioSource(float dt) {
	TODO
}

void SDL2AudioInputComponent::Play(const AudioSinkConfig& config, SystemSound& snd) {
	TODO
}



void SDL2AudioOutputComponent::Initialize() {
	aconfig.sample_pos = 0;
	SetAudioSyncInterval(1.5);
	if (VirtualSoundPtr == 0)
		VirtualSoundPtr = this;
	OBJ_CREATE
}

void SDL2AudioOutputComponent::Uninitialize() {
	if (VirtualSoundPtr == this)
		VirtualSoundPtr = 0;
	obj.Clear();
}

void SDL2AudioOutputComponent::RecvAudioSink(AudioSource& src, float dt) {
	av_sync_age += dt;
	aconfig.dt += dt;
	
	SystemSound& snd = obj->GetSystemSound(); //obj->BeginPlay();
	
	if (av_sync_age >= aconfig.sync_dt) {
		if (av_sync_age > 2 * aconfig.sync_dt)
			av_sync_age = aconfig.sync_dt;
		else
			av_sync_age = Modulus(av_sync_age, aconfig.sync_dt);
		
		aconfig.sample_pos = 0;
		aconfig.sync = true;
		
		src.Play(aconfig, snd);
		
		//obj->CommitPlay();
	}
	else if (!snd.IsQueueFull()) {
		aconfig.sync = false;
		src.Play(aconfig, snd);
	}
	/*else {
		obj->UndoPlay();
	}*/
}



void SDL2ScreenComponent::Initialize() {
	config.dt = 0;
	SetFPS(60);
	OBJ_CREATE
	auto* ev_comp = GetEntity().Find<SDL2EventsComponent>();
	if (ev_comp)
		ev = ev_comp->GetOOSDL2();
}

void SDL2ScreenComponent::Uninitialize() {
	ev = 0;
	obj.Clear();
}

void SDL2ScreenComponent::RecvScreenSink(ScreenSource& src, float dt) {
	frame_age += dt;
	config.dt += dt;
	
	if (frame_age >= config.fps_dt) {
		if (frame_age > 2 * config.fps_dt)
			frame_age = config.fps_dt;
		else
			frame_age = Modulus(frame_age, config.fps_dt);
		
		
		SystemDraw& draw = obj->BeginDraw();
		
		src.Render(config, draw);
		
		obj->CommitDraw();
	}
}

void SDL2ScreenComponent::SetTitle(String s) {
	if (obj)
		obj->SetTitle(s);
}



void SDL2EventsComponent::Initialize() {
	OBJ_CREATE
	
	Shared<EventSystem> ev_sys = GetEntity().GetMachine().Get<EventSystem>();
	if (!ev_sys)
		return;
	
	ev_sys -> AddControllable(*this);
}

void SDL2EventsComponent::Uninitialize() {
	obj.Clear();
	
	Shared<EventSystem> ev_sys = GetEntity().GetMachine().Get<EventSystem>();
	if (ev_sys)
		ev_sys->RemoveControllable(*this);
}

void SDL2EventsComponent::EmitControllerSource(float dt) {
	if (obj) {
		CtrlEvent e;
		while (obj->Poll(e)) {
			if (e.type == EVENT_SHUTDOWN) {
				GetMachine().SetNotRunning();
			}
			EmitCtrlEvent(e);
		}
	}
}

void SDL2EventsComponent::EmitCtrlEvent(const CtrlEvent& ev) {
	for(ControllerSink* sink : ControllerSource::GetSinks())
		sink->RecvCtrlEvent(ev);
}



void SDL2JoystickComponent::Initialize() {
	OBJ_CREATE
}

void SDL2JoystickComponent::Uninitialize() {
	obj.Clear();
}

void SDL2JoystickComponent::EmitControllerSource(float dt) {
	TODO
}

void SDL2JoystickComponent::EmitCtrlEvent(const CtrlEvent& ev) {
	TODO
}



void SDL2GameControllerComponent::Initialize() {
	OBJ_CREATE
}

void SDL2GameControllerComponent::Uninitialize() {
	obj.Clear();
}

void SDL2GameControllerComponent::EmitControllerSource(float dt) {
	TODO
}

void SDL2GameControllerComponent::EmitCtrlEvent(const CtrlEvent& ev) {
	TODO
}



void SDL2SensorComponent::Initialize() {
	OBJ_CREATE
}

void SDL2SensorComponent::Uninitialize() {
	obj.Clear();
}

void SDL2SensorComponent::EmitControllerSource(float dt) {
	TODO
}

void SDL2SensorComponent::EmitCtrlEvent(const CtrlEvent& ev) {
	TODO
}



void SDL2ImageComponent::Initialize() {
	OBJ_CREATE
}

void SDL2ImageComponent::Uninitialize() {
	obj.Clear();
}



void SDL2FontComponent::Initialize() {
	OBJ_CREATE
}

void SDL2FontComponent::Uninitialize() {
	obj.Clear();
}



SDL2ContextComponent::SDL2ContextComponent() {
	obj.Create();
}

void SDL2ContextComponent::Initialize() {
	Entity& e = GetEntity();
	
	SDL2TimerComponent* tim				= e.Find<SDL2TimerComponent>();
	SDL2AudioInputComponent* ain		= e.Find<SDL2AudioInputComponent>();
	SDL2AudioOutputComponent* aout		= e.Find<SDL2AudioOutputComponent>();
	SDL2ScreenComponent* scr			= e.Find<SDL2ScreenComponent>();
	SDL2EventsComponent* ev				= e.Find<SDL2EventsComponent>();
	
	SDL2JoystickComponent* joy			= e.Find<SDL2JoystickComponent>();
	SDL2GameControllerComponent* gc		= e.Find<SDL2GameControllerComponent>();
	SDL2SensorComponent* sens			= e.Find<SDL2SensorComponent>();
	SDL2ImageComponent* img				= e.Find<SDL2ImageComponent>();
	SDL2FontComponent* fnt				= e.Find<SDL2FontComponent>();
	
	#define AddObj(x) if(x) {obj->Add(x->GetObj()); comps.Add(x);}
	
	AddObj(tim);
	AddObj(ain);
	AddObj(aout);
	AddObj(scr);
	AddObj(ev);
	
	AddObj(joy);
	AddObj(gc);
	AddObj(sens);
	AddObj(img);
	AddObj(fnt);
	
	#undef AddObj
	
	if (!obj->Open()) {
		LOG("SDL2ContextComponent::Initialize: error: " << obj->GetLastError());
		GetMachine().SetNotRunning();
	}
}

void SDL2ContextComponent::Uninitialize() {
	obj->Close();
}


NAMESPACE_SPPP_END
