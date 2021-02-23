#ifndef _VirtualGui3DAlt_Direct_h_
#define _VirtualGui3DAlt_Direct_h_


NAMESPACE_SPPP_BEGIN


class DirectWindow :
	public Component<CoreWindow>,
	public ScreenInput,
	public SoundInput,
	public ControllerOutput,
	public ReadFileOutput,
	public VideoOutput
{
    ScreenOutput* screen_output_cb = NULL;
    bool is_open = false;
    
	static uint32 prev_ticks;
	int fps_limit = 0;
	float fps_dt = 0;
	float frame_age = 0;
	
public:
	
	IFACE_CB(ScreenInput);
	IFACE_CB(SoundInput);
	IFACE_CB(ControllerOutput);
	IFACE_CB(ReadFileOutput);
	IFACE_CB(VideoOutput);
	
	
	DirectWindow();
	
	void SetFPS(int fps) {fps_limit = fps; fps_dt = 1.0 / fps;}
	void Update(float dt);
	
	void Initialize() override;
	void Uninitialize() override;
	bool Attach(ScreenOutput& output) override;
	bool Attach(SoundOutput& output) override;
	
	void SetTitle(String title);
	
	Callback WhenClose;
	
};


NAMESPACE_SPPP_END


#endif
