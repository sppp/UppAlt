#include <EcsLib/EcsLib.h>


NAMESPACE_UPP

Sppp::Windows* Ctrl::wm;


Sppp::CoreWindow* Ctrl::GetWindow() {
	TopWindow* tw = GetTopWindow();
	return tw ? tw->GetWindow() : 0;
}


Image Ctrl::OverrideCursor(const Image& m) {
	TODO
}

void Ctrl::CloseTopCtrls() {
	LOG("TODO Ctrl::CloseTopCtrls");
}

#if 0

CoreWindow* Ctrl::GetWindow() {
	Ctrl* c = this;
	while (c) {
		CoreWindow* cw = dynamic_cast<CoreWindow*>(c);
		if (cw)
			return cw;
		c = c->GetParent();
	}
	return NULL;
}

#endif

Sppp::Windows* Ctrl::GetWindows() {
	ASSERT(wm);
	return wm;
}

END_UPP_NAMESPACE
