#include <EcsLib/EcsLib.h>


NAMESPACE_UPP

Sppp::CoreWindow* Ctrl::GetWindow() const {
	TopWindow* tw = GetTopWindow();
	return tw ? tw->GetWindow() : 0;
}


Image Ctrl::OverrideCursor(const Image& m) {
	TODO
}

END_UPP_NAMESPACE
