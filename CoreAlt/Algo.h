#ifndef _Core_Algo_h_
#define _Core_Algo_h_

#include <algorithm>

NAMESPACE_UPP_BEGIN

#undef min
#undef max
template <class T> const T& min(const T& a, const T& b) { return a < b ? a : b; }
template <class T> const T& max(const T& a, const T& b) { return a > b ? a : b; }

void GetSysSeedValues(int64* a, int64* b, int64* c);


class RNG : Moveable<RNG> {
	uint64 state[4];
	
	uint64 s_rotl(const uint64 x, int k) {
		return (x << k) | (x >> (64 - k)); // GCC/CLANG/MSC happily optimize this
	}
	
	uint64 sNext(uint64 *s) {
		const uint64 result_starstar = s_rotl(s[1] * 5, 7) * 9;
		const uint64 t = s[1] << 17;
	
		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];
	
		s[2] ^= t;
	
		s[3] = s_rotl(s[3], 45);
	
		return result_starstar;
	}
	
	void sSeed(uint64 *s) {
		for(int pass = 0; pass < 4; pass++) {
			for(int i = 0; i < 4; i++) {
				int64 a;
				int64 b;
				int64 c;
				GetSysSeedValues(&a, &b, &c);
				CombineHash h;
				h.Put64(a);
				h.Put64(b);
				h.Put64(c);
				s[i] ^= h;
			}
		}
	}
	
public:
	RNG() {Seed();}
	float  Randomf() {return (sNext(state) >> 11) * (1.f / (UINT64_C(1) << 53));}
	double Randomd() {return (sNext(state) >> 11) * (1.  / (UINT64_C(1) << 53));}
	uint64 Random(uint64 max) {return Get() % max;}
	uint64 Get() {return sNext(state);}
	void GetN(uint64* t, int n) {for(int i = 0; i < n; i++) t[i] = sNext(state);}
	operator uint64 () {return Get();}
	void Seed() {sSeed(state);}
	void Seed(uint32 seed) {
		for(int i = 0; i < 4; i++)
			state[i] = 12345678 + seed + i; // xoshiro does not work well if all is zero
	}
	
	static RNG& Local() {static thread_local RNG r; return r;}
};


inline dword Random() {return (dword)RNG::Local().Random(UINT32_MAX);}
inline void Random64(uint64* t, int n) {return RNG::Local().GetN(t, n);}
inline dword Random(dword n) {return RNG::Local().Random(n);}
inline uint64 Random64() {return RNG::Local().Get();}
inline uint64 Random64(uint64 n) {return RNG::Local().Random(n);}
inline double Randomf() {return RNG::Local().Randomf();}
inline void SeedRandom() {return RNG::Local().Seed();}
inline void SeedRandom(dword seed) {return RNG::Local().Seed(seed);}


template <class I, class T>
I FindIf(I begin, I end, std::function<T> fn) {
	while (begin != end) {
		if (fn(*begin))
			return begin;
		begin++;
	}
	return end;
}



template <class T, class K>
inline bool IsInheritedBy(const K& o) {return dynamic_cast<const T*>(&o);}

template <class T, class K>
inline T& Cast(K& o) {return dynamic_cast<T&>(o);}









class SpinLock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
    void Enter() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void Leave() {
        locked.clear(std::memory_order_release);
    }
    bool TryEnter() {
        return !locked.test_and_set(std::memory_order_acquire);
    }
};

struct AtomicBool {
	std::atomic<bool> value;
	AtomicBool() {value = 0;}
	AtomicBool(bool b) {value = b;}
	AtomicBool(const AtomicBool& ai) {value = (bool)ai.value;}
	operator bool() {return value;}
	bool operator = (bool b) {value = b; return b;}
	//int operator++(int) {return value++;}
	//int operator--(int) {return value--;}
};

struct AtomicInt {
	std::atomic<int> value;
	AtomicInt() {value = 0;}
	AtomicInt(int i) {value = i;}
	AtomicInt(const AtomicInt& ai) {value = (int)ai.value;}
	operator int() const {return value;}
	int operator = (int i) {value = i; return i;}
	int operator++() {return ++value;}
	int operator--() {return --value;}
	int operator++(int) {return value++;}
	int operator--(int) {return value--;}
};

struct FakeAtomicInt : Moveable<FakeAtomicInt> {
	SpinLock lock;
	int value;
	FakeAtomicInt() : value(0) {}
	FakeAtomicInt(int i) : value(i) {}
	FakeAtomicInt(const AtomicInt& ai) : value(ai.value) {}
	operator int() {return value;}
	int operator = (int i) {
		lock.Enter();
		value = i;
		lock.Leave();
		return i;
	}
	int operator++(int) {
		lock.Enter();
		int i = value++;
		lock.Leave();
		return i;
	}
	int operator--(int) {
		lock.Enter();
		int i = value--;
		lock.Leave();
		return i;
	}
	int Get() const {return value;}
};

typedef AtomicInt Atomic;

NAMESPACE_UPP_END

#endif
