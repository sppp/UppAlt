#include "Draw.h"


NAMESPACE_UPP_BEGIN

Image DefaultImages::Arrow;



void Image::MakeSysAccel() {
	TODO
}







Image StreamRaster::LoadFileAny(String path) {
	return Image(IMG_Load(path.Begin()));
}

NAMESPACE_UPP_END

