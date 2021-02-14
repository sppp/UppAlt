#include <EcsLib/EcsLib.h>


NAMESPACE_UPP

VirtualGui3DAlt* VirtualGui3DAltPtr;



Size SystemDraw::GetPageSize() const {
	if (!gui)
		return Size(0,0);
	return gui->GetSize();
}

END_UPP_NAMESPACE
