#include <EcsLib/EcsLib.h>


NAMESPACE_UPP


TopWindow::TopWindow() : wm(0), id(-1) {
	
}

void TopWindow::Init(Sppp::Windows* wm, Sppp::CoreWindow* cw, int id) {
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


END_UPP_NAMESPACE
