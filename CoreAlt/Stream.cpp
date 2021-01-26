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

void StdLogSetup(dword flags) {
	MultiStream& s = LogMulti();
	s.Clear();
	
	if (flags & LOG_FILE) {
		s.Add(Log());
	}
	if (flags & LOG_COUT) {
		s.Add(Cout());
	}
}

NAMESPACE_UPP_END
