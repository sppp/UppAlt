#include "Draw.h"


NAMESPACE_UPP_BEGIN

Image DefaultImages::Arrow;



void Image::MakeSysAccel() {
	TODO
}

const byte* Image::GetIter(int x, int y) const {
	if (!data) return 0;
	return data->img.GetData() + y * data->img.GetPitch() + x * data->img.GetStride();
}






Image StreamRaster::LoadFileAny(String path) {
	return Image(IMG_Load(path.Begin()));
}

NAMESPACE_UPP_END

