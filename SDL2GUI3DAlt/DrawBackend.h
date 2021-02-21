
#include <GL/glew.h>
#ifdef flagOPENVR
	#include <openvr.h>
#endif
#include <GL/gl.h>

#ifdef flagWIN32
	#include <SDL.h>
	#include <SDL_ttf.h>
	#include <SDL_image.h>
	#include <SDL_opengl.h><
	#include <SDL_syswm.h><
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_opengl.h>
#endif
#undef main


NAMESPACE_UPP

typedef TTF_Font		RawSysFont;
typedef SDL_Surface		RawSysImage;
typedef SDL_Texture		RawSysTexture;
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
	
	//SysImage() {}
	SysImage(RawSysImage* raw) : raw(raw) {++raw->refcount;}
	~SysImage() {Clear();}
	void Clear();
	int GetWidth() const {if (raw) return raw->w; return 0;}
	int GetHeight() const {if (raw) return raw->h; return 0;}
	int GetStride() const {return raw && raw->format ? raw->format->BytesPerPixel : 0;}
	int GetPitch() const {return raw ? raw->pitch : 0;}
	const unsigned char* GetData() const {return raw ? (const unsigned char*)raw->pixels : 0;}
	
};


struct SysAccelImage {
	int gl_tex = 0;
	
};

END_UPP_NAMESPACE
