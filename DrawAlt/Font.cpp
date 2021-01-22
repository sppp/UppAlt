#include "Draw.h"
#include <Sppp/Sppp.h>

NAMESPACE_UPP

Font Font::LoadFont(String dir, String name, int ptsize, int weight, bool italic) {
	String path = AppendFileName(dir, name);
	switch (weight) {
		case 0:	path += "-Thin";	break;
		case 1:	path += "-Regular";	break;
		case 2:	path += "-Medium";	break;
		case 3:	path += "-Bold";	break;
	}
	path += ".ttf";
	
	if (!FileExists(path)) {
		return Font();
	}
	
	FontRef* ref = new FontRef();
	
	DLOG("Opening font " << path);
	ref->font = TTF_OpenFont(path.Begin(), ptsize);
	ref->dir = dir;
	ref->name = name;
	ref->ptsize = ptsize;
	ref->weight = weight;
	ref->italic = italic;
	
	if (ref->font.IsEmpty()) {
		DLOG("Opening font failed: " << TTF_GetError());
	}
	return ref;
}

int __std_font_sz = 15;
Font __std_font;

Size GetStdFontSize() {
	Size sz(__std_font_sz, __std_font_sz);
	return sz;
}

Font GetStdFont(int size) {
	size = size < 0 ? GetStdFontSize().cy : size;
	if (__std_font.IsEmpty()) {
		__std_font = Font(Font::LoadFont(Sppp::ShareDirFile("fonts"), "Roboto", size));
	}
	return __std_font;
}

void SetStdFont(Font fnt) {
	__std_font = fnt;
}

Size GetTextSize(String s, Font fnt) {
	if (fnt.IsEmpty())
		return Size(0,0);
	Size sz;
	TTF_SizeText(fnt.GetSysFont()->GetRaw(), s.Begin(), &sz.cx, &sz.cy);
	return sz;
}

END_UPP_NAMESPACE
