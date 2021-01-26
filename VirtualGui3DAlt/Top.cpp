#include "Windows.h"

NAMESPACE_SPPP_BEGIN


TopWindow::TopWindow() : wm(0), id(-1) {
	
}

void TopWindow::Init(Windows* wm, CoreWindow* cw, int id) {
	this->wm = wm;
	this->cw = cw;
	this->id = id;
	
	if (title.IsEmpty())
		title = wm->GetTitle(id);
	else
		wm->SetTitle(id, title);
}

void TopWindow::Title(const String& title) {
	this->title = title;
	if (wm)
		wm->SetTitle(id, title);
}

void TopWindow::SetFrameRect(const Rect& r) {
	Ctrl::SetFrameRect(r);
	cw->SetFrameRect(r);
}


NAMESPACE_SPPP_END
