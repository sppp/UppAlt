#ifndef _OOSDL2_System_h_
#define _OOSDL2_System_h_

#include <EcsLib/EcsLib.h>

NAMESPACE_SPPP_BEGIN


class SDL2System : public System<SDL2System> {
	Sppp::OOSDL2::Context ctx;
	
public:
	SDL2System(Machine& m);
	
	
protected:
	friend class Font;
	friend class CoreWindow;
	
    bool Initialize() override;
    void Start() override;
    void Update(float dt) override;
    void Stop() override;
    void Uninitialize() override;
    
	
};


class SDL2TimerComponent :
	public Component<SDL2TimerComponent>
{
	One<OOSDL2::Timer> obj;
	
public:
	COPY_PANIC(SDL2TimerComponent);
	
	SDL2TimerComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::Timer* GetOOSDL2() {return &*obj;}
	
};

class SDL2AudioInputComponent :
	public Component<SDL2AudioInputComponent>,
	public AudioSource
{
	One<OOSDL2::AudioInput> obj;
	
public:
	COPY_PANIC(SDL2AudioInputComponent);
	IFACE_CB(AudioSource);
	
	SDL2AudioInputComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	void EmitAudioSource(float dt) override;
	void Play(const AudioSinkConfig& config, SystemSound& snd) override;
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::AudioInput* GetOOSDL2() {return &*obj;}
	
};

class SDL2AudioOutputComponent :
	public Component<SDL2AudioOutputComponent>,
	public AudioSink
{
	One<OOSDL2::AudioOutput> obj;
	SystemSound empty_snd;
    AudioSinkConfig aconfig;
	double av_sync_age = 0;
	
	
public:
	COPY_PANIC(SDL2AudioOutputComponent);
	IFACE_CB(AudioSink);
	
	SDL2AudioOutputComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	void RecvAudioSink(AudioSource& src, float dt) override;
	
	SystemSound&	BeginPlay() override {return obj ? obj->GetSystemSound() : empty_snd;}
	void			CommitPlay() override {}
	void			UndoPlay() override {ASSERT_(0, "UndoPlay is not implemented");}
	bool			IsAudioSampleFloating() override {return obj ? obj->IsSampleFloating() : 0;}
	int				GetAudioSampleRate() override {return obj ? obj->GetSampleRate() : 0;}
	int				GetAudioChannels() override {return obj ? obj->GetChannels() : 0;}
	int				GetAudioFrequency() override {return obj ? obj->GetFrequency() : 0;}
	int				GetAudioSampleSize() override {return obj ? obj->GetSampleSize() : 0;}
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::AudioOutput* GetOOSDL2() {return &*obj;}
	
	void SetAudioSyncInterval(double seconds) {aconfig.sync_dt = seconds;}
	
};

class SDL2ScreenComponent :
	public Component<SDL2ScreenComponent>,
	public ScreenSink
{
	One<OOSDL2::Screen> obj;
    ScreenSinkConfig config;
	OOSDL2::Events* ev = 0;
	double frame_age = 0;
	
public:
	COPY_PANIC(SDL2ScreenComponent);
	IFACE_CB(ScreenSink);
	
	SDL2ScreenComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	void RecvScreenSink(ScreenSource& src, float dt) override;
	void SetTitle(String s) override;
	uint32 GetTickCount() override {return ev ? ev->GetTickCount() : 0;}
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::Screen* GetOOSDL2() {return &*obj;}
	
	void SetFPS(int fps) {config.fps = fps; config.fps_dt = 1.0 / fps;}
	
};

class SDL2EventsComponent :
	public Component<SDL2EventsComponent>,
	public ControllerSource
{
	One<OOSDL2::Events> obj;
	
public:
	COPY_PANIC(SDL2EventsComponent);
	IFACE_CB(ControllerSource);
	
	SDL2EventsComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	void EmitControllerSource(float dt) override;
	void EmitCtrlEvent(const CtrlEvent& ev) override;
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::Events* GetOOSDL2() {return &*obj;}
	
};

class SDL2JoystickComponent :
	public Component<SDL2JoystickComponent>,
	public ControllerSource
{
	One<OOSDL2::Joystick> obj;
	
public:
	COPY_PANIC(SDL2JoystickComponent);
	IFACE_CB(ControllerSource);
	
	SDL2JoystickComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	void EmitControllerSource(float dt) override;
	void EmitCtrlEvent(const CtrlEvent& ev) override;
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::Joystick* GetOOSDL2() {return &*obj;}
	
};

class SDL2GameControllerComponent :
	public Component<SDL2GameControllerComponent>,
	public ControllerSource
{
	One<OOSDL2::GameController> obj;
	
public:
	COPY_PANIC(SDL2GameControllerComponent);
	IFACE_CB(ControllerSource);
	
	SDL2GameControllerComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	void EmitControllerSource(float dt) override;
	void EmitCtrlEvent(const CtrlEvent& ev) override;
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::GameController* GetOOSDL2() {return &*obj;}
	
};

class SDL2SensorComponent :
	public Component<SDL2SensorComponent>,
	public ControllerSource
{
	One<OOSDL2::Sensor> obj;
	
public:
	COPY_PANIC(SDL2SensorComponent);
	IFACE_CB(ControllerSource);
	
	SDL2SensorComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	void EmitControllerSource(float dt) override;
	void EmitCtrlEvent(const CtrlEvent& ev) override;
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::Sensor* GetOOSDL2() {return &*obj;}
	
};

class SDL2ImageComponent :
	public Component<SDL2ImageComponent>,
	public ReadFileSource
{
	One<OOSDL2::Image> obj;
	
public:
	COPY_PANIC(SDL2ImageComponent);
	IFACE_CB(ReadFileSource);
	
	SDL2ImageComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::Image* GetOOSDL2() {return &*obj;}
	
};

class SDL2FontComponent :
	public Component<SDL2FontComponent>,
	public ReadFileSource
{
	One<OOSDL2::Font> obj;
	
public:
	COPY_PANIC(SDL2FontComponent);
	IFACE_CB(ReadFileSource);
	
	SDL2FontComponent() = default;
	
	void Initialize() override;
	void Uninitialize() override;
	
	OOSDL2::Component& GetObj() {return *obj;}
	OOSDL2::Font* GetOOSDL2() {return &*obj;}
	
};

class SDL2ContextComponent :
	public Component<SDL2ContextComponent>
{
	One<OOSDL2::Context> obj;
	Vector<ComponentBase*> comps;
	
public:
	COPY_PANIC(SDL2ContextComponent);
	
	SDL2ContextComponent();
	
	void Initialize() override;
	void Uninitialize() override;
	
	OOSDL2::Context* GetOOSDL2() {return &*obj;}
	
};



struct StandaloneWindow : EntityPrefab<
	SDL2TimerComponent,
	SDL2EventsComponent,
	//SDL2AudioInputComponent,
	SDL2AudioOutputComponent,
	SDL2ScreenComponent,
	SDL2JoystickComponent,
	SDL2GameControllerComponent,
	SDL2SensorComponent,
	SDL2ImageComponent,
	SDL2FontComponent,
	SDL2ContextComponent,
	DirectWindow,
	Connector>
{
    static ComponentMap Make(ComponentStore& store)
    {
        auto components = EntityPrefab::Make(store);
		return components;
    }
};

struct StandaloneLineIn : EntityPrefab<
	SDL2AudioInputComponent,
	SDL2ContextComponent,
	Connector>
{
    static ComponentMap Make(ComponentStore& store)
    {
        auto components = EntityPrefab::Make(store);
		return components;
    }
};


NAMESPACE_SPPP_END

#endif
