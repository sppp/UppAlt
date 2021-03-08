#ifndef _VirtualGui3DAlt_Direct_h_
#define _VirtualGui3DAlt_Direct_h_


NAMESPACE_SPPP_BEGIN


class DirectWindow :
	public Component<CoreWindow>,
	public ScreenSource,
	public SoundSource,
	public ControllerSource,
	public ReadFileSink,
	public VideoSink
{
    ScreenSink* screen_output_cb = NULL;
    bool is_open = false;
    
	static uint32 prev_ticks;
	int fps_limit = 0;
	float fps_dt = 0;
	float frame_age = 0;
	
	Vector<ControllerSink*> ctrl_sinks;
	
public:
	
	IFACE_CB(ScreenSource);
	IFACE_CB(SoundSource);
	IFACE_CB(ControllerSource);
	IFACE_CB(ReadFileSink);
	IFACE_CB(VideoSink);
	
	
	DirectWindow();
	
	void SetFPS(int fps) {fps_limit = fps; fps_dt = 1.0 / fps;}
	void Update(float dt);
	
	void Initialize() override;
	void Uninitialize() override;
	bool Link(ScreenSink& output) override;
	bool Link(SoundSink& output) override;
	
	bool Link(ControllerSink& ctrl_out) override;
	bool Unlink(ControllerSink& ctrl_out) override;
	
	void EmitCtrlEvent(const CtrlEvent& e) override;
	
	void SetTitle(String title);
	
	Callback WhenClose;
	
};


NAMESPACE_SPPP_END


#endif
