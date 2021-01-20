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

Stream& Log();
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

template <class I, class Less>
inline void OrderIter2__(I a, I b, const Less& less)
{
	if(less(*b, *a))
		IterSwap(a, b);
}

template <class I, class Less>
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
			if(less(*best, *--ptr)) {  // best holds, scan for better candidate
				do
					if(ptr == begin) { // best is the final minimum
						IterSwap(begin, best);
						++begin;
						goto NEXT_ITEM;
					}
				while(less(*best, *--ptr));
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


template <class I, class Less>
void Sort__(I l, I h, const Less& less)
{
	for(;;) {
		int count = int(h - l);
		if(count < 2)
			return;
		if(count < 8) {                         // Final optimized SelectSort
			FinalSort__(l, h, less);
			return;
		}
		int pass = 4;
		for(;;) {
			I middle = l + (count >> 1);        // get the middle element
			OrderIter2__(l, middle, less);      // sort l, middle, h-1 to find median of 3
			OrderIter2__(middle, h - 1, less);
			OrderIter2__(l, middle, less);      // median is now in middle
			IterSwap(l + 1, middle);            // move median pivot to l + 1
			I ii = l + 1;
			for(I i = l + 2; i != h - 1; ++i)   // do partitioning; already l <= pivot <= h - 1
				if(less(*i, *(l + 1)))
					IterSwap(++ii, i);
			IterSwap(ii, l + 1);                // put pivot back in between partitions
			I iih = ii;
			while(iih + 1 != h && !less(*ii, *(iih + 1))) // Find middle range of elements equal to pivot
				++iih;
			if(pass > 5 || min(ii - l, h - iih) > (max(ii - l, h - iih) >> pass)) { // partition sizes ok or we have done max attempts
				if(ii - l < h - iih - 1) {       // recurse on smaller partition, tail on larger
					Sort__(l, ii, less);
					l = iih + 1;
				}
				else {
					Sort__(iih + 1, h, less);
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
	Sort__(vector.Begin().Get(), vector.End().Get(), sorter);
}



template <class T> struct StdLess {bool operator()(const T& a, const T& b) const {return a < b;}};
template <class T> struct StdGreater {bool operator()(const T& a, const T& b) const {return a > b;}};



inline void* MemoryAlloc(size_t size) {return malloc(size);}
inline void MemoryFree(void* ptr) {free(ptr);}







String MD5String(String s);
String BZ2Compress(String s, int level=9, bool allow_empty=false);
String BZ2Decompress(String s, bool allow_fail=false);
String HexEncode(String s);


NAMESPACE_UPP_END

#endif
