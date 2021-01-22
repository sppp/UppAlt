#include "Local.h"


NAMESPACE_UPP

void Ctrl::InitFB() {
	Ctrl::InitTimer();
	
	static StaticRect x;
	x.Color(Cyan());
	SetDesktop(x);
}

void Ctrl::ExitFB() {
	TODO
}

void Ctrl::SetDesktopSize(Size sz) {
	if(desktop)
		desktop->SetRect(sz);
	invalid = true;
	SyncTopWindows();
}

void Ctrl::SyncTopWindows()
{
	for(int i = 0; i < topctrl.GetCount(); i++) {
		TopWindow *w = dynamic_cast<TopWindow *>(topctrl[i]);
		if(w)
			w->SyncRect();
	}
}

void Ctrl::SetDesktop(Ctrl& q) {
	desktop = &q;
	/*desktop->SetOpen(true);
	desktop->NewTop();
	invalid = true;*/
}

END_UPP_NAMESPACE
