#include <SDL2/SDL_ttf.h>



NAMESPACE_UPP

typedef TTF_Font		RawSysFont;
typedef SDL_Surface		RawSysImage;
typedef SDL_Color		SysColor;

struct SysFont {
	RawSysFont* raw = 0;
	
	SysFont() {}
	SysFont(RawSysFont* f) : raw(f) {}
	~SysFont() {Clear();}
	
	void Clear();
	bool IsEmpty() const {return raw == 0;}
	
	RawSysFont* GetRaw() const {return raw;}
	RawSysImage* RenderTextBlended(const char* s, SysColor c) {return TTF_RenderText_Blended(raw, s, c);}
};

struct SysImage {
	RawSysImage* raw = 0;
	
	SysImage() {}
	SysImage(RawSysImage* raw) : raw(raw) {}
	~SysImage() {Clear();}
	void Clear();
	int GetWidth() const {if (raw) return raw->w; return 0;}
	int GetHeight() const {if (raw) return raw->h; return 0;}
	
};


struct SysAccelImage {
	int gl_tex = 0;
	
};

END_UPP_NAMESPACE
