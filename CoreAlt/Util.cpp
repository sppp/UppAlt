#include "Core.h"

#ifdef flagWIN32
#include <shlwapi.h>
#endif

#ifdef flagPOSIX
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <openssl/md5.h>
#include <bzlib.h>

NAMESPACE_UPP_BEGIN

int64 usecs(int64 prev)
{
	auto p2 = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(p2.time_since_epoch()).count() - prev;
}

int msecs(int prev)
{
	auto p2 = std::chrono::steady_clock::now();
	return (int)std::chrono::duration_cast<std::chrono::milliseconds>(p2.time_since_epoch()).count() - prev;
}








void Uuid::New() {
	Random64(v, 2);
}

String Uuid::ToString() const {
	String s;
	union {
		uint64 u64;
		byte u8[8];
	};
	int c = 0;
	for(int i = 0; i < 2; i++) {
		u64 = v[i];
		for(int j = 0; j < 8; j++) {
			byte b = u8[j];
			if (c == 8 || c == 12 || c == 16 || c == 20)
				s.Cat('-');
			s.Cat(HexDigitLower(b >> 4));
			s.Cat(HexDigitLower(0xF & b));
			c+=2;
		}
	}
	return s;
}















Vector<String> __cmdline;
void ParseCommandLine(const char* cmdline) {
	__cmdline = Split(cmdline, " ");
}

void ParseCommandLine(int argc, const char** argv) {
	__cmdline.Clear();
	for(int i = 1; i < argc; i++)
		__cmdline.Add(argv[i]);
}

const Vector<String>& CommandLine() {return __cmdline;}

int __exitcode;
int GetExitCode() {return __exitcode;}
void SetExitCode(int i) {__exitcode = i;}



String sDataPath;
String config_path;

void SetCoreArg(String key, String value) {
	if (key.IsEmpty())
		return;
	
	LOG("core-arg: \"" << key << "\" -> \"" << value << "\"");
	if (key == "datapath") {
		ASSERT(DirectoryExists(value));
		sDataPath = value;
	}
	else if (key == "config") {
		ASSERT(DirectoryExists(value));
		config_path = value;
	}
}






String GetExeDirFile(String filename) {
	return AppendFileName(GetFileDirectory(GetExeFilePath()), filename);
}

String GetEnv(String id)
{
	if (id.IsEmpty())
		return String();
	#ifdef flagWIN32
	size_t len = 0;
	char homedir[2048];
	getenv_s(&len, &homedir[0], 2048, id.Begin());
	return String(homedir, len);
	#else
	return getenv(id.Begin());
	#endif
}





void ReadCoreCmdlineArgs() {
	for(String arg : __cmdline) {
		int i = arg.Find("=");
		if (i >= 0) {
			String key = arg.Left(i);
			String value = arg.Mid(i+1);
			if (key.Left(2) == "-E") {
				key = key.Mid(2);
				SetCoreArg(key, value);
			}
		}
	}
}






void SetDataPath(String path) {
	sDataPath = path;
}

String GetDataDirectory() {
	if(sDataPath.GetCount())
		return sDataPath;
	return GetEnv("UPP_MAIN__");
}

String GetDataFile(String filename) {
	if(sDataPath.GetCount())
		return AppendFileName(sDataPath, filename);
	String s = GetEnv("UPP_MAIN__");
	return s.GetCount() ? AppendFileName(s, filename) : GetExeDirFile(filename);
}

bool FileExists(String path) {
	#ifdef flagWIN32
	#ifndef flagUWP
	return PathFileExistsA(path.Begin());
	#else
	struct stat stat_info;
	if (stat(path.Begin(), &stat_info) != 0)
		return false;
	return true;
	#endif
	#else
	return access( path.Begin(), F_OK ) != -1;
	#endif
}

bool DirectoryExists(String path) {
	#ifdef flagWIN32
	#ifndef flagUWP
	DWORD dwAttrib = GetFileAttributes(path.Begin());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	#else
	#error TODO
	#endif
	#else
	DIR* dir = opendir(path.Begin());
	if (dir) {
	    closedir(dir);
	    return true;
	}
	return false;
	#endif
}

Vector<String> local_file_dirs;

String FindLocalFile(String filename) {
	String exe_dir_file = GetExeDirFile(filename);
	if (FileExists(exe_dir_file))
		return exe_dir_file;
	
	for(int i = 0; i < local_file_dirs.GetCount(); i++) {
		String dir_file = AppendFileName(local_file_dirs[i], filename);
		if (FileExists(dir_file) || DirectoryExists(dir_file))
			return dir_file;
	}
	
	return ConfigFile(filename);
}

void AddLocalFileDirectory(String dir) {
	local_file_dirs.Add(dir);
}

bool __is_verbose;
bool IsVerbose() {return __is_verbose;}
void SetVerbose(bool b) {__is_verbose = b;}











#if defined flagWIN32 && defined flagGUI
HINSTANCE __hinst_cur, __hinst_prev;
bool __nCmdShow;
void SetWin32Instances(HINSTANCE inst, HINSTANCE prev, bool nCmdShow) {
	__hinst_cur = inst;
	__hinst_prev = prev;
	__nCmdShow = nCmdShow;
}
HINSTANCE GetWin32Instance() {return __hinst_cur;}
HINSTANCE GetWin32PrevInstance() {return __hinst_prev;}
bool GetWin32CmdShow() {return __nCmdShow;}
#endif














FindFile::FindFile() {
	
}

bool FindFile::Search(String path) {
	TODO
}

bool FindFile::Next() {
	TODO
}

bool FindFile::IsDirectory() const {
	TODO
}

String FindFile::GetPath() const {
	TODO
}

String FindFile::GetName() const {
	TODO
}


































void FillVector(Vector<double>& v) {
	int first = -1;
	for(int i = 0; i < v.GetCount(); i++)
		if (v[i] != 0.0)
			{first = i; break;}
	if (first < 0)
		return;
	double left_pad = v[first];
	for(int i = first-1; i >= 0; i--)
		v[i] = left_pad;
	double prev = left_pad;
	for(int i = first+1; i < v.GetCount(); i++) {
		double& d = v[i];
		if (!d)
			d = prev;
		else
			prev = d;
	}
}

String UpperBegin(String s) {
	String o;
	int is_upper = true;
	for(int i = 0; i < s.GetCount(); i++) {
		int chr = s[i];
		if (is_upper) {
			o.Cat(ToUpper(chr));
			is_upper = false;
		}
		else
			o.Cat(ToLower(chr));
		if (chr == ' ')
			is_upper = true;
	}
	return o;
}

String GetTimeDurationString(Time begin, Time end) {
	String out;
	int64 total = end.Get() - begin.Get();
	bool neg = false;
	if (total < 0) {
		neg = true;
		total *= -1;
	}
	int sec = total % 60;
	total /= 60;
	int min = total % 60;
	total /= 60;
	int hours = total % 24;
	total /= 24;
	int days = total % 30;
	total /= 30;
	int months = total % 12;
	total /= 12;
	int64 years = total;
	if (years)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d years"), years);}
	if (months)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d months"), months);}
	if (days)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d days"), days);}
	if (hours)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d hours"), hours);}
	if (min)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d minutes"), min);}
	if (sec)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d seconds"), sec);}
	
	if (neg) out = "-" + out;
	return out;
}



bool StreamCopy(Stream& dest, Stream& src) {
	dest.Seek(0);
	src.Seek(0);
	byte buf[1024];
	int64 size_left = src.GetSize();
	while (size_left > 0) {
		int block = size_left < 1024 ? (int)size_left : 1024;
		int got = src.Get(buf, block);
		if (got != block)
			return false;
		dest.Put(buf, block);
		size_left -= block;
	}
	return true;
}


String shared_dir;

void SetSharedDirectory(String d) {shared_dir = d;}
String GetSharedDirectory() {return shared_dir;}
String SharedFile(String file) {
	if (shared_dir.IsEmpty())
		return ConfigFile(file);
	else
		return AppendFileName(shared_dir, file);
}




/*

void StringParser::PassChar(int chr, bool do_pass_white) {
	if (!IsChar(chr))
		throw Exc("unexpected char");
	cursor++;
	if (do_pass_white)
		PassWhite();
}

bool StringParser::Char(int chr) {
	int curchr = s[cursor];
	if (curchr == chr) {
		cursor++;
		PassWhite();
		return true;
	}
	return false;
}

int StringParser::ReadInt() {
	String i;
	while (IsDigit(s[cursor])) {
		i.Cat(s[cursor++]);
	}
	PassWhite();
	return StrInt(i);
}

String StringParser::ReadId() {
	String id;
	while (cursor < s.GetCount()) {
		int chr = s[cursor];
		if (IsSpace(chr))
			break;
		
		if (chr == '\\') {
			cursor++;
			chr = s[cursor];
		}
		if (IsAlpha(chr) || chr == '_' || IsDigit(chr)) {
			id.Cat(chr);
			cursor++;
		}
		else
			break;
	}
	PassWhite();
	return id;
}

int StringParser::GetChar() {
	return s[cursor++];
}

int StringParser::PeekChar() {
	return s[cursor];
}

bool StringParser::IsChar(int chr) {
	return s[cursor] == chr;
}

void StringParser::PassWhite() {
	while (cursor < s.GetCount()) {
		int chr = s[cursor];
		if (IsSpace(chr))
			cursor++;
		else
			break;
	}
}

*/





String MD5String(String s) {
	unsigned char result[MD5_DIGEST_LENGTH+1];
    MD5((const unsigned char*)s.Begin(), s.GetCount(), result);
    result[MD5_DIGEST_LENGTH] = 0;
    return HexEncode(String((const char*)result, MD5_DIGEST_LENGTH));
}

String BZ2Compress(String s, int level, bool allow_empty) {
	const int chunk_size = 1 << 16;
	bz_stream STREAM;
	Vector<char> dst;
	level = max(1, min(9, level));
	
	int src_len = s.GetCount();
	int dst_len = 0;
	const char* src_buf = s.Begin();

    // Check parameters
    if (!src_len) {
        if (!allow_empty)
            return String();
        src_buf = NULL;
    }
    
    STREAM.bzalloc = NULL;
    STREAM.bzfree  = NULL;
    STREAM.opaque  = NULL;
    int errcode = BZ2_bzCompressInit(&STREAM, level, 0, 0);
   
    if (errcode == BZ_OK) {
        dst.SetCount(chunk_size);
    
        STREAM.next_in   = (char*)s.Begin();
        STREAM.avail_in  = 0;
        STREAM.next_out  = dst.GetData();
        STREAM.avail_out = chunk_size;

        size_t left = src_len;
        do {
            if (STREAM.avail_in == 0) {
                STREAM.avail_in = left > chunk_size ? chunk_size : (unsigned int)left;
                left -= STREAM.avail_in;
            }
            if (STREAM.avail_out == 0) {
                int count = dst.GetCount();
                dst.SetCount(count + chunk_size);
                STREAM.next_out  = dst.GetData() + count;
                STREAM.avail_out = chunk_size;
            }
            errcode = BZ2_bzCompress(&STREAM, left ? BZ_RUN : BZ_FINISH);
            if (left == 0 && STREAM.avail_out > 0)
                break;
        }
        while (errcode == BZ_RUN_OK);

        BZ2_bzCompressEnd(&STREAM);
        
        int dst_len = STREAM.next_out - (char*)dst.GetData();
        ASSERT(dst_len >= 0);
        dst.SetCount(dst_len);
        
        if (errcode == BZ_RUN_OK || errcode == BZ_STREAM_END) {
            String s;
            s.SetData(dst.Begin(), dst.GetCount());
            return s;
        }
    }
	
	return String();
}

String BZ2Decompress(String s, bool allow_fail) {
	const int chunk_size = 1 << 16;
	bz_stream STREAM;
	Vector<char> dst;
	int src_len = s.GetCount();
    if (!src_len)
        return String();
    
    STREAM.bzalloc = NULL;
    STREAM.bzfree  = NULL;
    STREAM.opaque  = NULL;
    int errcode = BZ2_bzDecompressInit(&STREAM, 0, 0);
   
    if (errcode == BZ_OK) {
        dst.SetCount(chunk_size);
        
        STREAM.next_in   = (char*)s.Begin();
        STREAM.avail_in  = 0;
        STREAM.next_out  = dst.GetData();
        STREAM.avail_out = chunk_size;

        size_t left = src_len;
        do {
            if (STREAM.avail_in == 0) {
                STREAM.avail_in = left > chunk_size ? chunk_size : (unsigned int)left;
                left -= STREAM.avail_in;
            }
            if (STREAM.avail_out == 0) {
                int count = dst.GetCount();
                dst.SetCount(count + chunk_size);
                STREAM.next_out  = dst.GetData() + count;
                STREAM.avail_out = chunk_size;
            }
            errcode = BZ2_bzDecompress(&STREAM);
            if (left == 0 && STREAM.avail_out > 0)
                break;
        }
        while (errcode == BZ_OK);
		
        BZ2_bzDecompressEnd(&STREAM);
        
        int dst_len = STREAM.next_out - (char*)dst.GetData();
        ASSERT(dst_len >= 0);
        dst.SetCount(dst_len);
        
        if (errcode == BZ_OK || errcode == BZ_STREAM_END || allow_fail) {
            String s;
            s.SetData(dst.Begin(), dst.GetCount());
            return s;
        }
    }
	
	return String();
}

String HexEncode(String s) {
	String out;
	out.Reserve(s.GetCount()*2);
	int count = s.GetCount();
	const char* c = s.Begin();
	for(int i = 0; i < count; i++, c++) {
		int a = *c >> 4;
		int b = *c & 0xF;
		out.Cat(HexDigitLower(a));
		out.Cat(HexDigitLower(b));
	}
	return out;
}







bool Load(Callback1<Stream&> x, Stream& s) {
	TODO
}

void Store(Callback1<Stream&> x, Stream& s) {
	TODO
}

String GetGlobalConfigData(const char* name) {
	TODO
}

void SetGlobalConfigData(const char* name, String data) {
	TODO
}

bool LoadFromGlobal(Callback1<Stream&> x, const char *name) {
	StringStream ss(GetGlobalConfigData(name));
	return ss.IsEof() || Load(x, ss);
}

void StoreToGlobal(Callback1<Stream&> x, const char *name) {
	StringStream ss;
	Store(x, ss);
	SetGlobalConfigData(name, ss);
}






String GetHomeDirectory() {
	#ifdef flagPOSIX
	struct passwd *pw = getpwuid(getuid());
	return pw->pw_dir;
	#elif defined flagWIN32
	const char* path = getenv("USERPROFILE");
	return path;
	#else
	#error Not implemented yet
	#endif
}


String GetHomeDirFile(const char *fp) {
	return AppendFileName(GetHomeDirectory(), fp);
}




void AppInit__(int argc, const char** argv, const char** environ) {
	::Upp::__ForceSetThreadId(MAIN_THREAD_ID);
	::Upp::SeedRandom();
	
	#ifdef flagWIN32
	char chr[512]; GetModuleFileNameA(NULL, chr, 512);
	::SetExeFilePath(chr);
	::ParseCommandLine((LPSTR)argv);
	#else
	if (argc > 0)
		::Upp::SetExeFilePath(argv[0]);
	::Upp::ParseCommandLine(argc, argv);
	#endif
	
	::Upp::StdLogSetup(LOG_FILE);
	::Upp::ReadCoreCmdlineArgs();
	::Upp::RunInitBlocks();
	
	#if defined flagDEBUG && defined flagPOSIX
	AddLocalFileDirectory(GetHomeDirectory() + "/upphub/UppCommon/share");
	#endif
}

int AppExit__() {
	::Upp::Thread::ShutdownThreads();
	::Upp::RunExitBlocks();
	return ::Upp::GetExitCode();
}


NAMESPACE_UPP_END
