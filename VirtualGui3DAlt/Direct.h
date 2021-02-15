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
	
	
public:
	
	IFACE_CB(ScreenInput);
	IFACE_CB(SoundInput);
	IFACE_CB(ControllerOutput);
	IFACE_CB(ReadFileOutput);
	IFACE_CB(VideoOutput);
	
	
	DirectWindow();
	
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
