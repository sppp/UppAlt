#ifndef _Core_Chrono_h_
#define _Core_Chrono_h_

NAMESPACE_UPP_BEGIN


struct Date {
	short year;
	uint8 month, day;
	
	
	Date() : year(0), month(0), day(0) {}
	Date(int y, int m, int d) : year(y), month(m), day(d) {}
	
	void Set(int y, int m, int d);
	void Set(int64 scalar);
	int64 Get() const;
	bool operator==(const Date& d) const {return year == d.year && month == d.month && day == d.day;}
	
	String ToString() const;
	int64 ToInt() const {return Get();}
	
	static const int* DaysInMonth();
	static const int* MonthDayOffset();
	static bool IsLeapYear(int year);
	static int GetDaysOfMonth(int m, int y);
	
};

struct Time : public Date {
	uint8 hour, min, sec;
	
	
	Time() {}
	Time(const Time& t) { MemoryCopy(this, &t, sizeof(Time)); }
	Time(int y, int m, int d) {Set(y,m,d,0,0,0);}
	Time(int y, int m, int d, int h, int min, int s) {Set(y,m,d,h,min,s);}
	
	void Set(int y, int mon, int d, int h, int m, int s);
	void Set(int64 scalar);
	void SetFromStamp(int seconds);
	int64 Get() const {return Date::Get() * (int64)24 * 3600 + hour * 3600 + min * 60 + sec;}
	uint32 GetHashValue() {return sec + 32 * (min + 32 * (hour + 16 * (day + 32 * (month + 8 * year))));}
	int GetStamp() const;
	
	bool operator==(const Time& t) const {return hour == t.hour && min == t.min && sec == t.sec;}
	
	String ToString() const;
	String ToDiffString() const;
	int64 ToInt() const {return Get();}
	static Time GetSys();
};

void GetSysTimeValue(short& year, uint8& mon, uint8& day, uint8& hour, uint8& min, uint8& sec);
Time GetSysTime();

String GetTimeDurationString(Time begin, Time end);


inline Time operator+(const Time& t, int64 seconds) {
	Time o;
	o.Set(t.Get() + seconds);
	return o;
}


class TimeStop {
	std::chrono::high_resolution_clock::time_point start;
	
public:
	TimeStop() {}
		
	void Reset();
	int Elapsed() const;
	double ElapsedSeconds() const;
	int ResetElapsed();
	double ResetElapsedSeconds();
	String ToString() const;
};








Time GetSysTime();

String GetTimeString(Time t);
dword Timestamp(const Time& t);
dword TimestampNow();
Time TimeFromTimestamp(int64 seconds);
String TimeDiffString(int64 seconds);

bool IsLeapYear(int year);


void  SetTimeCallback(int delay_ms, Callback cb, void *id = NULL); // delay_ms < 0 -> periodic
void  KillTimeCallback(void *id);
bool  ExistsTimeCallback(void *id);
dword GetTimeClick();
void AddGlobalTicks(uint32 ticks);
uint32 GetGlobalTicks();

inline void  PostCallback(Callback cb, void *id = NULL)  { SetTimeCallback(1, cb, id); }


class TimeCallback
{
public:
	~TimeCallback()                               { Kill(); (void)dummy; }

	void Set(int delay, Callback cb)     { SetTimeCallback(delay, cb, this); }
	void Post(Callback cb)               { PostCallback(cb, this); }
	void Kill()                                   { KillTimeCallback(this); }
	void KillSet(int delay, Callback cb) { Kill(); Set(delay, cb); }
	void KillPost(Callback cb)           { Kill(); Post(cb); }

private:
	byte dummy;
};





struct OnlineFrequency {
	TimeStop timer;
	int ticks = 0;
	
	OnlineFrequency() {}
	void Reset() {timer.Reset(); ticks = 0;}
	double Get() const {return (double)ticks / timer.Elapsed() * 1000;}
	void Tick() {ticks++;}
	
	operator double() const {return Get();}
	double operator++() {Tick(); return Get();}
};

struct OnlineFrequencyWindow {
	OnlineFrequency freq0, freq1;
	bool which = 0;
	double interval = 0.5;
	
	OnlineFrequencyWindow() {}
	OnlineFrequency& GetActive()   {return which ? freq1 : freq0;}
	OnlineFrequency& GetInactive() {return which ? freq0 : freq1;}
	void SwitchActive() {which = !which;}
	void Tick() {
		freq0.Tick();
		freq1.Tick();
		if (GetInactive().timer.Elapsed() >= interval) {
			SwitchActive();
			GetActive().Reset();
		}
	}
	double Get() const {return (which ? freq1 : freq0).Get();}
	
	operator double() const {return Get();}
	double operator++() {Tick(); return Get();}
};





int64 CurrentTime();
uint64 NanoSeconds();
uint64 MilliSeconds();

String GetTimeDurationString(Time begin, Time end);

NAMESPACE_UPP_END

#endif
