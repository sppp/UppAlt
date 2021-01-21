#ifndef _Core_Stream_h_
#define _Core_Stream_h_

#ifdef flagPOSIX
inline int fopen_s(FILE **f, const char *name, const char *mode) {
    int ret = 0;
    ASSERT(f);
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}
#endif

NAMESPACE_UPP_BEGIN

class Stream {

public:

	virtual String Get(int size) { return ""; }
	virtual bool IsOpen() const { return false; }

	virtual bool IsLoading() { return false; }
	virtual bool IsStoring() { return false; }
	virtual bool IsEof() { return false; }

	virtual Stream& operator << (String s) { return *this; }
	virtual Stream& operator << (int i) { return *this; }
	virtual int Put(const void* mem, int size) { return 0; }
	virtual int Put(char c) {return Put(&c, 1);}
	virtual int Put(const String& s) { return Put(s.Begin(), s.GetCount()); }
	virtual int Get(void* mem, int size) { return 0; }
	virtual int64 GetCursor() { return 0; }
	virtual void Seek(int64 pos) {}
	virtual int64 GetSize() const {return 0;}
	
	virtual void Flush() {};
	
	String GetLine(int max = UINT16_MAX) {
		String out;
		while (!IsEof() && out.GetCount() < max) {
			char chr;
			Get(&chr, 1);
			if (chr == '\n')
				break;
			out.Cat(chr);
		}
		return out;
	}
	
	template <class T>
	Stream& operator%(T& o) {o.Serialize(*this); return *this;}
	
	
	int Put(char chr, int count) {
		if (count <= 4) {int wrote = 0; for(int i = 0; i < count; i++) wrote += Put(&chr, 1); return wrote;}
		else {String s; s.Cat(chr, count); return Put(s);}
	}
	int Get(Huge& huge, int size);
	
};

#define DEFAULT_SERIALIZER(type) template <> inline Stream& Stream::operator%(type& o) {\
	if (IsLoading()) Get(&o, sizeof(type)); \
	else Put(&o, sizeof(type)); \
	return *this; \
}

DEFAULT_SERIALIZER(bool)
DEFAULT_SERIALIZER(int8)
DEFAULT_SERIALIZER(uint8)
DEFAULT_SERIALIZER(int16)
DEFAULT_SERIALIZER(uint16)
DEFAULT_SERIALIZER(int32)
DEFAULT_SERIALIZER(uint32)
DEFAULT_SERIALIZER(int64)
DEFAULT_SERIALIZER(uint64)
DEFAULT_SERIALIZER(float)
DEFAULT_SERIALIZER(double)

class FileStream : public Stream {
	
protected:
	mutable FILE* s = NULL;
	
public:
	FileStream() {}
	FileStream(FILE* s) : s(s) {}
	
};

class FileIn : public FileStream {

public:
	FileIn() {}
	FileIn(FILE* s) : FileStream(s) {}
	FileIn(String path) {Open(path);}
	~FileIn() { Close(); }

	bool IsLoading() override { return true; }

	bool IsOpen() const override { return s != NULL; }
	bool IsEof() override { if (!s) return true; return feof(s); }

	bool Open(String path) {
		if (s) fclose(s);
		fopen_s(&s, path.Begin(), "rb");
		return s != NULL;
	}

	void Close() {
		if (s) fclose(s); s = NULL;
	}

	String Get(int size) override {
		if (!s || !size) return String();
		ASSERT(size > 0);
		int pos = ftell(s);
		Vector<char> v;
		v.SetCount(size + 1, 0);
		char* it = v.Begin();
		int left = size;
		int total_read = 0;
		while (left > 0) {
			int read_size = (int)fread(it, 1, left, s);
			left -= read_size;
			it += read_size;
			total_read += read_size;
			if (!read_size)
				break;
		}
		String out;
		out.SetData(v.Begin(), total_read+1);
		return out;
	}

	String GetText(int size) {
		if (!s) return "";
		Vector<char> v;
		v.SetCount(size+1);
		v[size] = 0;
		size_t read_size = fread(v.Begin(), 1, size, s);
		return v.GetData();
	}

	int Get(void* mem, int size) override {
		if (!s) return 0;
		int64 ret = fread(mem, size, 1, s) * size;
		ASSERT(ret < INT_MAX);
		return (int)ret;
	}

	int64 GetCursor() override { if (!s) return 0; return ftell(s); }

	int64 GetSize() const override {
		if (!s) return 0;
		long pos = ftell(s);
		fseek(s, 0, SEEK_END);
		int size = ftell(s);
		fseek(s, pos, SEEK_SET);
		return size;
	}

	void Seek(int64 pos) override { fseek(s, (long)pos, SEEK_SET); }

};

String LoadFile(String path);


class FileOut : public FileStream {

public:
	FileOut() {}
	FileOut(FILE* s) : FileStream(s) {}
	FileOut(String path) {Open(path);}
	~FileOut() { Close(); }

	bool IsStoring() override { return true; }

	bool IsOpen() const override { return s != NULL; }
	bool IsEof() override { if (!s) return true; return feof(s); }

	bool Open(String path) {
		if (s) fclose(s);
		#ifdef flagMSC
		s = _fsopen(path.Begin(), "w+b", _SH_DENYWR);
		#else
		s = fopen(path.Begin(), "w+b");
		#endif
		return s != NULL;
	}

	void Close() {
		if (s) { fflush(s); fclose(s); s = NULL; }
	}

	Stream& operator << (String str) override {
		Put(str.Begin(), str.GetCount());
		return *this;
	}
	Stream& operator << (int i) override {
		String str = IntStr(i);
		Put(str.Begin(), str.GetCount());
		return *this;
	}

	int Put(const void* mem, int size) override {
		if (!s) return 0;
		int64 ret = fwrite(mem, size, 1, s) * size;
		ASSERT(ret < INT_MAX);
		return (int)ret;
	}
	int Put(char c) override { return Put(&c, 1); }

	void Flush() override { if (s) fflush(s); }
	
	int64 GetCursor() override { if (!s) return 0; return ftell(s); }

	int64 GetSize() const override {
		if (!s) return 0;
		long pos = ftell(s);
		fseek(s, 0, SEEK_END);
		int size = ftell(s);
		fseek(s, pos, SEEK_SET);
		return size;
	}

};

extern FileIn cin;
extern FileOut cout, cerr;

class StringStream : public Stream {
	Vector<char> s;
	int64 cursor = 0;
	bool is_storing = true;
	
public:
	StringStream() {}
	StringStream(String s) {this->s.SetCount(s.GetCount()); memcpy(this->s.Begin(), s.Begin(), this->s.GetCount());}

	bool IsLoading() override { return !is_storing; }
	bool IsStoring() override { return is_storing; }
	bool IsEof() override { return cursor >= s.GetCount(); }
	
	void SetStoring() {is_storing = true;}
	void SetLoading() {is_storing = false;}
	
	Stream& operator << (String str) override {
		Put(str.Begin(), str.GetCount());
		return *this;
	}
	Stream& operator << (int i) override {
		String str = IntStr(i);
		Put(str.Begin(), str.GetCount());
		return *this;
	}
	int Put(const void* mem, int size) override {
		int64 end = cursor + size;
		ASSERT(end < INT_MAX);
		if (end > s.GetCount())
			s.SetCount((int)end);
		char* dst = s.Begin() + cursor;
		const char* src = (const char*)mem;
		MemoryCopy(dst, src, size);
		cursor += size;
		return size;
	}
	int Put(char c) override { return Put(&c, 1); }
	int Put(const String& s) override { return Put(s.Begin(), s.GetCount()); }
	int Get(void* mem, int size) override {
		int64 sz = min((int64)size, (int64)s.GetCount() - cursor);
		if (sz <= 0) return 0;
		ASSERT(sz < INT_MAX);
		char* b = (char*)mem;
		MemoryCopy(b, s.Begin() + cursor, (int)sz);
		cursor += sz;
		return (int)sz;
	}
	int64 GetCursor() override { return cursor; }
	int64 GetSize() const override {return s.GetCount();}

	void Seek(int64 i) override { cursor = i; }
	String GetResult() const { String s; s.SetData(s.Begin(), s.GetCount()); return s;}
	
	operator String() const {return GetResult();}
	
};

class MemReadStream : public Stream {
	const char* buf;
	int64 size = 0;
	int64 cursor = 0;
	
public:
	MemReadStream(const char* buf, int64 size) : buf(buf), size(size) {}

	bool IsLoading() override { return true; }
	bool IsStoring() override { return false; }
	bool IsEof() override { return cursor >= size; }
	
	int Get(void* mem, int size) override {
		int64 sz = min((int64)size, size - cursor);
		if (sz <= 0) return 0;
		ASSERT(sz < INT_MAX);
		char* b = (char*)mem;
		MemoryCopy(b, buf + cursor, (int)sz);
		cursor += sz;
		return (int)sz;
	}
	int64 GetCursor() override { return cursor; }
	void Seek(int64 i) override { cursor = i; }
	int64 GetSize() const override {return size;}
};



template <class T>
void StringT<T>::Serialize(Stream& str) {
	uint32 len = 0;
	if (str.IsLoading()) {
		Clear();
		str.Get(&len, sizeof(len));
		count = len;
		if (count < buf_size) {
			str.Get(buf, len*sizeof(T));
			buf[count] = 0;
		}
		else {
			T* buf = (T*)malloc(sizeof(T) * (count + 1));
			str.Get(buf, len*sizeof(T));
			buf[count] = 0;
			BIG = new String0T(buf, count + 1);
			is_big = true;
		}
	}
	else {
		len = GetCount();
		str.Put(&len, sizeof(len));
		str.Put(Begin(), len*sizeof(T));
	}
}

template <class T>
void Vector<T>::Serialize(Stream& str) {
	uint32 len = 0;
	if (str.IsLoading()) {
		str.Get(&len, sizeof(len));
		SetCount(len);
	}
	else {
		len = GetCount();
		str.Put(&len, sizeof(len));
	}
	for(T& o : *this)
		str % o;
}

template <class T>
void LoadFromFile(T& o, String path) {
	FileIn fin(path);
	if (!fin.IsOpen()) return;
	fin % o;
}

template <class T>
void StoreToFile(T& o, String path) {
	FileOut fout(path);
	if (!fout.IsOpen()) return;
	fout % o;
}






NAMESPACE_UPP_END

#endif
