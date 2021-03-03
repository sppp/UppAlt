#ifndef _Core_Util_h_
#define _Core_Util_h_

NAMESPACE_UPP_BEGIN

void Panic(String s);
void Assert(bool b, String s="Assertion failed");
void AssertFalse(bool b, String s="Assertion failed");

template <class T>
T& PtrRef(T* o, String throw_msg) {
	if (!o) throw throw_msg;
	return *o;
}

MultiStream& LogMulti();
Stream& Log();
Stream& LogFile();
Stream& Cout();
Stream& Cerr();
Stream& Cin();

inline String ReadStdIn() {return Cin().GetLine();}

void SetExeFilePath(String s);
String GetExeFilePath();
String GetHomeDir();
String ConfigFile(String file_name);
String GetFileName(String path);
String GetFileTitle(String path);
String GetFileDirectory(String path);
String AppendFileName(String a, String b);
String GetParentDirectory(String path, int steps=1);
String GetFileExt(String path);
void RealizeDirectory(String dir);
void DeleteFile(String path);

inline bool IsFin(float f) {return ::isfinite(f);}


/*struct StringParser {
	String s;
	int cursor = 0;

	StringParser(String s) : s(s) {}

	void PassChar(int chr, bool do_pass_white = true);
	bool Char(int chr);
	int ReadInt();
	String ReadId();
	int GetChar();
	int PeekChar();
	bool IsChar(int chr);
	void PassWhite();
};*/



void ParseCommandLine(int argc, const char** argv);
void ParseCommandLine(const char* cmdline);
const Vector<String>& CommandLine();
int GetExitCode();
void SetExitCode(int i);
void SetCoreArg(String key, String value);
String GetExeDirFile(String filename);
String GetEnv(String id);
void ReadCoreCmdlineArgs();
void SetDataPath(String path);
String GetDataDirectory();
String GetDataFile(String filename);
bool FileExists(String path);
bool DirectoryExists(String path);
String FindLocalFile(String filename);
void AddLocalFileDirectory(String dir);
bool IsVerbose();
void SetVerbose(bool b=true);

#if defined flagWIN32 && defined flagGUI
void SetWin32Instances(HINSTANCE inst, HINSTANCE prev, bool nCmdShow);
HINSTANCE GetWin32Instance();
HINSTANCE GetWin32PrevInstance();
bool GetWin32CmdShow();
#endif

template <class T>
inline T& Single() {
	static T o;
	return o;
}


int64 usecs(int64 prev = 0);
int msecs(int prev = 0);




struct Uuid {
	uint64 v[2];

	Uuid() {}

	unsigned GetHashValue() const { CombineHash ch; ch.Put64(v[0]); ch.Put64(v[1]); return ch;}

	void     New();
	
	String ToString() const;
	static Uuid Create() { Uuid uuid; uuid.New(); return uuid; }
};




class FindFile {
	
	
public:
	typedef FindFile CLASSNAME;
	FindFile();
	
	bool Search(String path);
	bool Next();
	
	bool IsDirectory() const;
	String GetPath() const;
	String GetName() const;
};




template <class I>
inline void IterSwap(I a, I b) { if(a != b) Swap(*a, *b); }

template <class R, class I, class Less>
inline void OrderIter2__(I a, I b, const Less& less)
{
	if(less(R(b)(), R(a)()))
		IterSwap(a, b);
}

template <class R, class I, class Less>
inline void FinalSort__(I begin, I end, const Less& less) {
	if(begin == end)
		return;
	I last = end;
	--last;
	while(!(begin == last)) {
		I best = last;
		I next = last;
		I ptr = last;
		for(;;) {
			if(less(R(best)(), R(--ptr)())) {  // best holds, scan for better candidate
				do
					if(ptr == begin) { // best is the final minimum
						IterSwap(begin, best);
						++begin;
						goto NEXT_ITEM;
					}
				while(less(R(best)(), R(--ptr)()));
				if(ptr == begin) { // begin is the final minimum, best is 2nd least
					IterSwap(++begin, best);
					++begin;
					break;
				}
				next = ptr; // mark position after new best as the new end of sorted array
				++next;     // it will hold only if all subseqent iterations define new best (descending order)
			}
			else
			if(ptr == begin) { // begin is the final minimum
				begin = next;  // and everything is sorted up to next
				break;
			}
			best = ptr;
		}
	NEXT_ITEM:
		;
	}
}

template <class R, class I, class Less>
void Sort__(I l, I h, const Less& less)
{
	for(;;) {
		int count = int(h - l);
		if(count < 2)
			return;
		if(count < 8) {                         // Final optimized SelectSort
			FinalSort__<R>(l, h, less);
			return;
		}
		int pass = 4;
		for(;;) {
			I middle = l + (count >> 1);        // get the middle element
			OrderIter2__<R>(l, middle, less);      // sort l, middle, h-1 to find median of 3
			OrderIter2__<R>(middle, h - 1, less);
			OrderIter2__<R>(l, middle, less);      // median is now in middle
			IterSwap(l + 1, middle);            // move median pivot to l + 1
			I ii = l + 1;
			for(I i = l + 2; i != h - 1; ++i)   // do partitioning; already l <= pivot <= h - 1
				if(less(R(i)(), R(l + 1)()))
					IterSwap(++ii, i);
			IterSwap(ii, l + 1);                // put pivot back in between partitions
			I iih = ii;
			while(iih + 1 != h && !less(R(ii)(), R(iih + 1)())) // Find middle range of elements equal to pivot
				++iih;
			if(pass > 5 || min(ii - l, h - iih) > (max(ii - l, h - iih) >> pass)) { // partition sizes ok or we have done max attempts
				if(ii - l < h - iih - 1) {       // recurse on smaller partition, tail on larger
					Sort__<R,I,Less>(l, ii, less);
					l = iih + 1;
				}
				else {
					Sort__<R,I,Less>(iih + 1, h, less);
					h = ii;
				}
				break;
			}
			IterSwap(l, l + (int)Random(count));     // try some other random elements for median pivot
			IterSwap(middle, l + (int)Random(count));
			IterSwap(h - 1, l + (int)Random(count));
			pass++;
		}
	}
}

template <class V, class S>
void Sort(V& vector, const S& sorter) {
	if (vector.GetCount() < 2)
		return;
	Sort__<typename V::RefGet, typename V::ElPtr, S>(vector.Begin().GetElPtr(), vector.End().GetElPtr(), sorter);
}





template <class I, class J>
inline void IterSwapIndex(I a, I b, J va, J vb) { if (a != b) {Swap(*a, *b); Swap(*va, *vb);}}

template <class I, class J, class Less>
inline void OrderIterIndex__(I a, I b, J va, J vb, const Less& less)
{
	if(less(*b, *a)) {
		IterSwapIndex(a, b, va, vb);
	}
}

template <class I, class J, class Less>
inline void FinalSortIndex__(I begin, I end, J vbegin, J vend, const Less& less) {
	if(begin == end)
		return;
	I last = end;   --last;
	J vlast = vend; --vlast;
	while(!(begin == last)) {
		I best = last;
		I next = last;
		I ptr = last;
		J vbest = vlast;
		J vnext = vlast;
		J vptr = vlast;
		for(;;) {
			if(less(*best, *--ptr)) {  // best holds, scan for better candidate
				do if(ptr == begin) { // best is the final minimum
					IterSwapIndex(begin, best, vbegin, vbest);
					++begin;
					++vbegin;
					goto NEXT_ITEM;
				}
				while(less(*best, *--ptr));
				if(ptr == begin) { // begin is the final minimum, best is 2nd least
					IterSwapIndex(++begin, best, ++vbegin, vbest);
					++begin;
					++vbegin;
					break;
				}
				next = ptr; // mark position after new best as the new end of sorted array
				vnext = vptr;
				++next;     // it will hold only if all subseqent iterations define new best (descending order)
				++vnext;
			}
			else
			if(ptr == begin) { // begin is the final minimum
				begin = next;  // and everything is sorted up to next
				vbegin = vnext;
				break;
			}
			best = ptr;
			vbest = vptr;
		}
	NEXT_ITEM:
		;
	}
}

template <class I, class J, class Less>
void SortIndex__(I l, I h, J vl, J vh, const Less& less)
{
	for(;;) {
		int count = int(h - l);
		if(count < 2)
			return;
		if(count < 8) {                         // Final optimized SelectSort
			FinalSortIndex__(l, h, vl, vh, less);
			return;
		}
		int pass = 4;
		for(;;) {
			I middle = l + (count >> 1);        // get the middle element
			J vmiddle = vl + (count >> 1);
			OrderIterIndex__(l, middle, vl, vmiddle, less);      // sort l, middle, h-1 to find median of 3
			OrderIterIndex__(middle, h - 1, vmiddle, vh - 1, less);
			OrderIterIndex__(l, middle, vl, vmiddle, less);      // median is now in middle
			IterSwapIndex(l + 1, middle, vl + 1, vmiddle);        // move median pivot to l + 1
			I ii = l + 1;
			J vii = vl + 1;
			{
				I i = l + 2;
				J vi = vl + 2;
				for(; i != h - 1; ++i, ++vi)   // do partitioning; already l <= pivot <= h - 1
					if(less(*i, *(l + 1)))
						IterSwapIndex(++ii, i, ++vii, vi);
			}
			IterSwapIndex(ii, l + 1, vii, vl + 1);          // put pivot back in between partitions
			I iih = ii;
			J viih = vii;
			// Find middle range of elements equal to pivot
			while(iih + 1 != h && !less(*ii, *(iih + 1))) {
				++iih;
				++viih;
			}
			// partition sizes ok or we have done max attempts
			if(pass > 5 || min(ii - l, h - iih) > (max(ii - l, h - iih) >> pass)) {
				if(ii - l < h - iih - 1) {       // recurse on smaller partition, tail on larger
					SortIndex__(l, ii, vl, vii, less);
					l = iih + 1;
					vl = viih + 1;
				}
				else {
					SortIndex__(iih + 1, h, viih + 1, vh, less);
					h = ii;
					vh = vii;
				}
				break;
			}
			int r0 = (int)Random(count);
			int r1 = (int)Random(count);
			int r2 = (int)Random(count);
			IterSwapIndex(l, l + r0, vl, vl + r0);     // try some other random elements for median pivot
			IterSwapIndex(middle, l + r1, vmiddle, vl + r1);
			IterSwapIndex(h - 1, l + r2, vh - 1, vl + r2);
			pass++;
		}
	}
}

template <class V, class S>
void SortIndex(V& idx, const S& sorter) {
	if (idx.GetCount() < 2)
		return;
	auto begin = idx.Begin().GetIter();
	auto end = idx.End().GetIter();
	SortIndex__(begin.a, end.a, begin.b, end.b, sorter);
}







template <class H, class I, class J>
inline void IterSwapKey(H ha, H hb, I a, I b, J va, J vb) { if (a != b) {Swap(*ha, *hb); Swap(*a, *b); Swap(*va, *vb);}}

template <class H, class I, class J, class Less>
inline void OrderIterKey2__(H ha, H hb, I a, I b, J va, J vb, const Less& less)
{
	if(less(*b, *a)) {
		IterSwapKey<H,I,J>(ha, hb, a, b, va, vb);
	}
}

template <class H, class I, class J, class Less>
inline void FinalSortKey__(H hbegin, H hend, I begin, I end, J vbegin, J vend, const Less& less) {
	if(begin == end)
		return;
	H hlast = hend; --hlast;
	I last = end;   --last;
	J vlast = vend; --vlast;
	while(!(begin == last)) {
		H hbest = hlast;
		H hnext = hlast;
		H hptr = hlast;
		I best = last;
		I next = last;
		I ptr = last;
		J vbest = vlast;
		J vnext = vlast;
		J vptr = vlast;
		for(;;) {
			if(less(*best, *--ptr)) {  // best holds, scan for better candidate
				do if(ptr == begin) { // best is the final minimum
					IterSwapKey<H,I,J>(hbegin, hbest, begin, best, vbegin, vbest);
					++hbegin;
					++begin;
					++vbegin;
					goto NEXT_ITEM;
				}
				while(less(*best, *--ptr));
				if(ptr == begin) { // begin is the final minimum, best is 2nd least
					IterSwapKey<H,I,J>(++hbegin, hbest, ++begin, best, ++vbegin, vbest);
					++hbegin;
					++begin;
					++vbegin;
					break;
				}
				hnext = hptr; // mark position after new best as the new end of sorted array
				next = ptr;
				vnext = vptr;
				++hnext;     // it will hold only if all subseqent iterations define new best (descending order)
				++next;
				++vnext;
			}
			else
			if(ptr == begin) { // begin is the final minimum
				hbegin = hnext;  // and everything is sorted up to next
				begin = next;
				vbegin = vnext;
				break;
			}
			hbest = hptr;
			best = ptr;
			vbest = vptr;
		}
	NEXT_ITEM:
		;
	}
}

template <class H, class I, class J, class Less>
void SortKey__(H hl, H hh, I l, I h, J vl, J vh, const Less& less)
{
	for(;;) {
		int count = int(h - l);
		if(count < 2)
			return;
		if(count < 8) {                         // Final optimized SelectSort
			FinalSortKey__(hl, hh, l, h, vl, vh, less);
			return;
		}
		int pass = 4;
		for(;;) {
			H hmiddle = hl + (count >> 1);        // get the middle element
			I middle = l + (count >> 1);
			J vmiddle = vl + (count >> 1);
			OrderIterKey2__<H,I,J,Less>(hl, hmiddle, l, middle, vl, vmiddle, less);      // sort l, middle, h-1 to find median of 3
			OrderIterKey2__<H,I,J,Less>(hmiddle, hh - 1, middle, h - 1, vmiddle, vh - 1, less);
			OrderIterKey2__<H,I,J,Less>(hl, hmiddle, l, middle, vl, vmiddle, less);      // median is now in middle
			IterSwapKey<H,I,J>(hl + 1, hmiddle, l + 1, middle, vl + 1, vmiddle);        // move median pivot to l + 1
			H hii = hl + 1;
			I ii = l + 1;
			J vii = vl + 1;
			{
				H hi = hl + 2;
				I i = l + 2;
				J vi = vl + 2;
				for(; i != h - 1; ++i, ++vi)   // do partitioning; already l <= pivot <= h - 1
					if(less(*i, *(l + 1)))
						IterSwapKey<H,I,J>(++hii, hi, ++ii, i, ++vii, vi);
			}
			IterSwapKey<H,I,J>(hii, hl + 1, ii, l + 1, vii, vl + 1);          // put pivot back in between partitions
			H hiih = hii;
			I iih = ii;
			J viih = vii;
			// Find middle range of elements equal to pivot
			while(iih + 1 != h && !less(*ii, *(iih + 1))) {
				++hiih;
				++iih;
				++viih;
			}
			// partition sizes ok or we have done max attempts
			if(pass > 5 || min(ii - l, h - iih) > (max(ii - l, h - iih) >> pass)) {
				if(ii - l < h - iih - 1) {       // recurse on smaller partition, tail on larger
					SortKey__(hl, hii, l, ii, vl, vii, less);
					hl = hiih + 1;
					l = iih + 1;
					vl = viih + 1;
				}
				else {
					SortKey__(hiih + 1, hh, iih + 1, h, viih + 1, vh, less);
					hh = hii;
					h = ii;
					vh = vii;
				}
				break;
			}
			int r0 = (int)Random(count);
			int r1 = (int)Random(count);
			int r2 = (int)Random(count);
			IterSwapKey<H,I,J>(hl, hl + r0, l, l + r0, vl, vl + r0);     // try some other random elements for median pivot
			IterSwapKey<H,I,J>(hmiddle, hl + r1, middle, l + r1, vmiddle, vl + r1);
			IterSwapKey<H,I,J>(hh - 1, hl + r2, h - 1, l + r2, vh - 1, vl + r2);
			pass++;
		}
	}
}

template <class V, class S>
void SortByKey(V& map, const S& sorter) {
	if (map.GetCount() < 2)
		return;
	auto begin = map.Begin().GetIter();
	auto end = map.End().GetIter();
	SortKey__<typename V::HashPtr, typename V::KeyPtr, typename V::ValuePtr, S>(begin.a, end.a, begin.b.a, end.b.a, begin.b.b, end.b.b, sorter);
}





template <class H, class I, class J>
inline void IterSwapValue(H ha, H hb, I a, I b, J va, J vb) { if (a != b) {Swap(*ha, *hb); Swap(*a, *b); Swap(*va, *vb);}}

template <class H, class I, class J, class Less>
inline void OrderIterValue2__(H ha, H hb, I a, I b, J va, J vb, const Less& less)
{
	if(less(**b, **a)) {
		IterSwapValue<H,I,J>(ha, hb, a, b, va, vb);
	}
}

template <class H, class I, class J, class Less>
inline void FinalSortValue__(H hbegin, H hend, I begin, I end, J vbegin, J vend, const Less& less) {
	if(begin == end)
		return;
	H hlast = hend; --hlast;
	I last = end;   --last;
	J vlast = vend; --vlast;
	while(!(begin == last)) {
		H hbest = hlast;
		H hnext = hlast;
		H hptr = hlast;
		I best = last;
		I next = last;
		I ptr = last;
		J vbest = vlast;
		J vnext = vlast;
		J vptr = vlast;
		for(;;) {
			if(less(**best, **--ptr)) {  // best holds, scan for better candidate
				do if(ptr == begin) { // best is the final minimum
					IterSwapValue<H,I,J>(hbegin, hbest, begin, best, vbegin, vbest);
					++hbegin;
					++begin;
					++vbegin;
					goto NEXT_ITEM;
				}
				while(less(**best, **--ptr));
				if(ptr == begin) { // begin is the final minimum, best is 2nd least
					IterSwapValue<H,I,J>(++hbegin, hbest, ++begin, best, ++vbegin, vbest);
					++hbegin;
					++begin;
					++vbegin;
					break;
				}
				hnext = hptr; // mark position after new best as the new end of sorted array
				next = ptr;
				vnext = vptr;
				++hnext;     // it will hold only if all subseqent iterations define new best (descending order)
				++next;
				++vnext;
			}
			else
			if(ptr == begin) { // begin is the final minimum
				hbegin = hnext;  // and everything is sorted up to next
				begin = next;
				vbegin = vnext;
				break;
			}
			hbest = hptr;
			best = ptr;
			vbest = vptr;
		}
	NEXT_ITEM:
		;
	}
}

template <class H, class I, class J, class Less>
void SortValue__(H hl, H hh, I l, I h, J vl, J vh, const Less& less)
{
	for(;;) {
		int count = int(h - l);
		if(count < 2)
			return;
		if(count < 8) {                         // Final optimized SelectSort
			FinalSortValue__(hl, hh, l, h, vl, vh, less);
			return;
		}
		int pass = 4;
		for(;;) {
			H hmiddle = hl + (count >> 1);        // get the middle element
			I middle = l + (count >> 1);
			J vmiddle = vl + (count >> 1);
			OrderIterValue2__<H,I,J,Less>(hl, hmiddle, l, middle, vl, vmiddle, less);      // sort l, middle, h-1 to find median of 3
			OrderIterValue2__<H,I,J,Less>(hmiddle, hh - 1, middle, h - 1, vmiddle, vh - 1, less);
			OrderIterValue2__<H,I,J,Less>(hl, hmiddle, l, middle, vl, vmiddle, less);      // median is now in middle
			IterSwapValue<H,I,J>(hl + 1, hmiddle, l + 1, middle, vl + 1, vmiddle);        // move median pivot to l + 1
			H hii = hl + 1;
			I ii = l + 1;
			J vii = vl + 1;
			{
				H hi = hl + 2;
				I i = l + 2;
				J vi = vl + 2;
				for(; i != h - 1; ++i, ++vi)   // do partitioning; already l <= pivot <= h - 1
					if(less(**i, **(l + 1)))
						IterSwapValue<H,I,J>(++hii, hi, ++ii, i, ++vii, vi);
			}
			IterSwapValue<H,I,J>(hii, hl + 1, ii, l + 1, vii, vl + 1);          // put pivot back in between partitions
			H hiih = hii;
			I iih = ii;
			J viih = vii;
			// Find middle range of elements equal to pivot
			while(iih + 1 != h && !less(**ii, **(iih + 1))) {
				++hiih;
				++iih;
				++viih;
			}
			// partition sizes ok or we have done max attempts
			if(pass > 5 || min(ii - l, h - iih) > (max(ii - l, h - iih) >> pass)) {
				if(ii - l < h - iih - 1) {       // recurse on smaller partition, tail on larger
					SortValue__(hl, hii, l, ii, vl, vii, less);
					hl = hiih + 1;
					l = iih + 1;
					vl = viih + 1;
				}
				else {
					SortValue__(hiih + 1, hh, iih + 1, h, viih + 1, vh, less);
					hh = hii;
					h = ii;
					vh = vii;
				}
				break;
			}
			int r0 = (int)Random(count);
			int r1 = (int)Random(count);
			int r2 = (int)Random(count);
			IterSwapValue<H,I,J>(hl, hl + r0, l, l + r0, vl, vl + r0);     // try some other random elements for median pivot
			IterSwapValue<H,I,J>(hmiddle, hl + r1, middle, l + r1, vmiddle, vl + r1);
			IterSwapValue<H,I,J>(hh - 1, hl + r2, h - 1, l + r2, vh - 1, vl + r2);
			pass++;
		}
	}
}

template <class V, class S>
void SortByValue(V& map, const S& sorter) {
	if (map.GetCount() < 2)
		return;
	auto begin = map.Begin().GetIter();
	auto end = map.End().GetIter();
	SortValue__<typename V::HashPtr, typename V::ValuePtr, typename V::KeyPtr, S>(begin.a, end.a, begin.b.b, end.b.b, begin.b.a, end.b.a, sorter);
}



template <class T> struct StdLess {bool operator()(const T& a, const T& b) const {return a < b;}};
template <class T> struct StdGreater {bool operator()(const T& a, const T& b) const {return a > b;}};



inline void* MemoryAlloc(size_t size) {return malloc(size);}
inline void MemoryFree(void* ptr) {free(ptr);}







String MD5String(String s);
String BZ2Compress(String s, int level=9, bool allow_empty=false);
String BZ2Decompress(String s, bool allow_fail=false);
String HexEncode(String s);




class NoCopy {
private:
	NoCopy(const NoCopy&);
	void operator=(const NoCopy&);
public:
	NoCopy() {}
};



bool Load(Callback1<Stream&> x, Stream& s);
void Store(Callback1<Stream&> x, Stream& s);
String GetGlobalConfigData(const char* name);
void SetGlobalConfigData(const char* name, String data);
bool LoadFromGlobal(Callback1<Stream&> x, const char *name);
void StoreToGlobal(Callback1<Stream&> x, const char *name);


String GetHomeDirectory();
String GetHomeDirFile(const char *fp);

void AppInit__(int argc, const char** argv, const char** environ);
int AppExit__();


NAMESPACE_UPP_END

#endif
