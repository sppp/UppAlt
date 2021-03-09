#ifndef _VirtualGui3DAlt_Direct_h_
#define _VirtualGui3DAlt_Direct_h_


NAMESPACE_SPPP_BEGIN


class DirectWindow :
	public Component<CoreWindow>,
	public ScreenSink,
	public AudioSink,
	public ControllerSource
	//public ReadFileSink,
	//public VideoSink
{
    bool is_open = false;
    
    ScreenSinkConfig config;
    AudioSinkConfig aconfig;
	static uint32 prev_ticks;
	double frame_age = 0;
	double av_sync_age = 0;
	
	
public:
	
	IFACE_CB(ScreenSink);
	IFACE_CB(AudioSink);
	IFACE_CB(ControllerSource);
	//IFACE_CB(ReadFileSink);
	//IFACE_CB(VideoSink);
	
	
	DirectWindow();
	
	void SetFPS(int fps) {config.fps = fps; config.fps_dt = 1.0 / fps;}
	void SetAudioSyncInterval(double seconds) {aconfig.sync_dt = seconds;}
	
	void Initialize() override;
	void Uninitialize() override;
	
	void EmitCtrlEvent(const CtrlEvent& e) override;
	
	void RecvScreenSink(ScreenSource& src, float dt) override;
	void RecvAudioSink(AudioSource& src, float dt) override;
	
	
	void SetTitle(String title) override;
	
	Callback WhenClose;
	
};


NAMESPACE_SPPP_END


#endif
