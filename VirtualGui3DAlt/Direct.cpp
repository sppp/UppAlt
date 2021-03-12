#include <EcsLib/EcsLib.h>


NAMESPACE_SPPP_BEGIN

//uint32 DirectWindow::prev_ticks;


DirectWindow::DirectWindow() {
	
}



void DirectWindow::Initialize() {
	ASSERT(!scr);
	Entity& e = GetEntity();
	scr = e.FindScreenSink();
}

void DirectWindow::Uninitialize() {
	if (scr)
		WhenClose();
	
	
	scr = 0;
}




NAMESPACE_SPPP_END
