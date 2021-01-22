#include "Local.h"


NAMESPACE_UPP

void TopWindowFrame::SyncRect() {
	if(maximized) {
		Size sz = GetWorkArea().GetSize();
		if(GetRect().GetSize() != sz)
			SetRect(sz);
	}
}

void TopWindow::SyncRect() {
	frame->SyncRect();
	Rect r = frame->GetClient();
	if(r != GetRect())
		SetRect(r);
}

void TopWindow::State(int reason) {
	TODO
}

END_UPP_NAMESPACE
