#ifndef _VirtualGui3DAlt_Direct_h_
#define _VirtualGui3DAlt_Direct_h_


NAMESPACE_SPPP_BEGIN


class DirectWindow :
	public Component<CoreWindow>
{
    ScreenSink* scr = 0;
	//static uint32 prev_ticks;
	
	
public:
	
	//IFACE_CB(ScreenSink);
	//IFACE_CB(AudioSink);
	//IFACE_CB(ControllerSource);
	//IFACE_CB(ReadFileSink);
	//IFACE_CB(VideoSink);
	
	
	DirectWindow();
	
	void Initialize() override;
	void Uninitialize() override;
	
	/*void EmitCtrlEvent(const CtrlEvent& e) override;
	
	void RecvScreenSink(ScreenSource& src, float dt) override;
	void RecvAudioSink(AudioSource& src, float dt) override;
	
	void SetTitle(String title) override;*/
	
	Callback WhenClose;
	
};


NAMESPACE_SPPP_END


#endif
