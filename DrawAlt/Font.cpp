#include "Draw.h"


NAMESPACE_UPP

Size GetStdFontSize() {
	TODO
}

Font GetStdFont(int size) {
	size = size < 0 ? GetStdFontSize().cy : size;
	
	TODO
}

void SetStdFont(Font fnt) {
	TODO
}

Size GetTextSize(String s, Font fnt) {
	TODO
}

END_UPP_NAMESPACE
