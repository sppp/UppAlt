#include "Core.h"

NAMESPACE_UPP_BEGIN


int Stream::Get(Huge& h, int size)
{
	while(h.GetSize() < size) {
		int sz = (int)min((size_t)h.CHUNK, size - h.GetSize());
		int len = Get(h.AddChunk(), sz);
		if(len < h.CHUNK) {
			h.Finish(len);
			break;
		}
	}
	return h.GetSize();
}


NAMESPACE_UPP_END
