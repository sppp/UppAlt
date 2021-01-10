#include "Core.h"

NAMESPACE_UPP_BEGIN

void TestShared() {
	struct Dumber {
		Dumber() {LOG("Dumber ctor +++");}
		virtual ~Dumber() {LOG("Dumber dtor ---");}
	};
	struct Dumb : public Dumber {
		Dumb() {LOG("Dumb ctor ++++++");}
		~Dumb() {LOG("Dumb dtor ------");}
	};
	
	
	#define DUMPDUMB(x, y) LOG(#x ": " #y ": refs " << (y.GetBase() ? (int)y.GetBase()->refs : -1) << ", " << IntStr64((uint64)y.GetBase()));
	
	Shared<Dumb> s0a;
	DUMPDUMB(0, s0a);
	{
		Shared<Dumber> s1a;
		Shared<Dumb> s1b;
		DUMPDUMB(1, s1a);
		DUMPDUMB(2, s1b);
		
		s1b = Shared<Dumb>().WrapObject(new Dumb());
		DUMPDUMB(3, s1b);
		
		s1a = s1b.template As<Dumber>();
		DUMPDUMB(4, s1a);
		
		{
			Shared<Dumber> s2;
			DUMPDUMB(5, s2);
			s2 = s1a;
			DUMPDUMB(6, s2);
			s0a = s2.template As<Dumb>();
			DUMPDUMB(7, s0a);
		}
		DUMPDUMB(8, s0a);
	}
	DUMPDUMB(9, s0a);
	
}
















Huge::Huge()
{
	size = 0;
}

byte *Huge::AddChunk()
{
	size += CHUNK;
	return data.Add().data;
}

void Huge::Finish(int last_chunk_size)
{
	size = size - CHUNK + last_chunk_size;
}

void Huge::Get(void *t_, size_t pos, size_t sz) const
{
	ASSERT(pos + sz <= size);
	int blki = int(pos / CHUNK);
	size_t blkpos = pos & (CHUNK - 1);
	byte *t = (byte *)t_;
	
	while(sz > 0) {
		size_t m = min(sz, CHUNK - blkpos);
		memcpy(t, data[blki].data + blkpos, m);
		t += m;
		sz -= m;
		blkpos = 0;
		blki++;
	}
}

String Huge::Get() const
{
	if(size >= INT_MAX)
		Panic("String is too big!");
	String s;
	void* mem = s.GetWritableData(size);
	Get(mem, 0, size);
	return s;
}

NAMESPACE_UPP_END
