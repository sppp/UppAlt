#include "SDL2GUI3DAlt.h"

NAMESPACE_UPP_BEGIN

void SysFont::Clear() {
	if (raw && Sppp::OOSDL2::IsSDL2Open())
		TTF_CloseFont(raw);
	raw = 0;
}

void SysImage::Clear() {
	if (raw && Sppp::OOSDL2::IsSDL2Open()) {
		if (--raw->refcount == 0)
			SDL_FreeSurface(raw);
	}
	raw = 0;
}

NAMESPACE_UPP_END
