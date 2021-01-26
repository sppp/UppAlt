#include "Local.h"


NAMESPACE_UPP

uint32 Ctrl::prev_ticks;
bool Ctrl::invalid;

void Ctrl::InitFB() {
	prev_ticks = VirtualGui3DAltPtr->GetTickCount();
	invalid = false;
}

void Ctrl::ExitFB() {
	
}

void Ctrl::SetDesktopSize(Size sz) {
	LOG("todo: SetDesktopSize " << sz.ToString());
}

void Ctrl::EventLoop(Ctrl *ctrl) {
	bool quit = false;
	ProcessEvents(&quit);
	while (!quit && !Thread::IsShutdownThreads()) {
		//SyncCaret();
		GuiSleep(20);
		ProcessEvents(&quit);
	}
}

void Ctrl::GuiSleep(int ms) {
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("GuiSleep");
	int level = LeaveGuiMutexAll();
	VirtualGui3DAltPtr->WaitEvent(ms);
	EnterGuiMutex(level);
}

bool Ctrl::ProcessEvents(bool *quit) {
	bool ret = ProcessEvent(quit);
	while(ProcessEvent(quit))
		;
	TimerProc(VirtualGui3DAltPtr->GetTickCount());
	//SweepMkImageCache();
	DoPaint();
	return ret;
}

bool Ctrl::ProcessEvent(bool *quit) {
	LLOG("@ ProcessEvent");
	ASSERT(IsMainThread());
	//if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
	//	ReleaseCtrlCapture();
	bool ret = VirtualGui3DAltPtr->ProcessEvent(quit);
	//DefferedFocusSync();
	//SyncCaret();
	//SyncTopWindows();
	return ret;
}

void Ctrl::TimerProc(dword time) {
	uint32 ticks = SDL_GetTicks();
	uint32 diff = ticks - prev_ticks;
	if (diff > 0) {
		AddGlobalTicks(diff);
		prev_ticks = ticks;
	}
}

void Ctrl::DoPaint() {
	if(invalid) {
		invalid = false;
		SystemDraw& draw = VirtualGui3DAltPtr->BeginDraw();
		PaintScene(draw);
		//PaintCaretCursor(draw);
		VirtualGui3DAltPtr->CommitDraw();
	}
}

void Ctrl::PaintScene(SystemDraw& draw) {
	TODO
}

END_UPP_NAMESPACE
