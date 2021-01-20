#ifndef _Core_Thread_h_
#define _Core_Thread_h_

NAMESPACE_UPP_BEGIN


inline int CPU_Cores() {return std::thread::hardware_concurrency();}

class Thread {
	One<std::thread> t;
	Callback cb;
	
	static bool shutdown;
	static AtomicInt thrd_count;
	
public:
	Thread() {thrd_count++;}
	~Thread() {thrd_count--;}


	static void Start(Callback cb) {
		Thread* t = new Thread();
		t->cb = cb;
		t->t = new std::thread([t, cb]() {
			cb.Execute();
			delete t;
		});
	}
	
	template <class T> static void Start(T& fn) {
		Thread* t = new Thread();
		t->cb.Clear();
		t->t = new std::thread([t, fn]() {
			fn();
			delete t;
		});
	}
	
	void Run(Callback cb);
	
	void Wait() {t->join();}
	void Detach() {t->detach();}
	
	static bool IsShutdownThreads() {return shutdown;}
	static void ShutdownThreads();
	
};

class Mutex {
	std::mutex m;
public:
	Mutex() {}
	
	void Enter() {m.lock();}
	void Leave() {m.unlock();}
	bool TryEnter() {return m.try_lock();}
	
};

class MutexLocker {
	Mutex* m;
public:
	MutexLocker(Mutex& m) : m(&m) {m.Enter();}
	~MutexLocker() {m->Leave();}
};

class shared_mutex {
    std::atomic<int> refcount{0};
public:
    void lock() {
        int val;
        do {
            val = 0; // Can only take a write lock when refcount == 0
        } while (!refcount.compare_exchange_weak(val, -1, std::memory_order_acquire));
        // can memory_order_relaxed be used if only a single thread takes write locks ?
    }
    void unlock() {
        refcount.store(0, std::memory_order_release);
    }
    void lock_shared() {
        int val;
        do {
            do {
                val = refcount.load(std::memory_order_relaxed);

            } while (val == -1); // spinning until the write lock is released

        } while (!refcount.compare_exchange_weak(val, val+1, std::memory_order_acquire));
    }
    void unlock_shared()  {
        refcount.fetch_sub(1, std::memory_order_relaxed);
    }
};


class RWMutex {
	shared_mutex m;
public:
	RWMutex() {}
	
	void EnterWrite() {m.lock();}
	void LeaveWrite() {m.unlock();}
	void EnterRead() {m.lock_shared();}
	void LeaveRead() {m.unlock_shared();}
};

class RWMutexReadLocker {
	RWMutex* m;
public:
	RWMutexReadLocker(RWMutex& m) : m(&m) {m.EnterRead();}
	~RWMutexReadLocker() {m->LeaveRead();}
};

class RWMutexWriteLocker {
	RWMutex* m;
public:
	RWMutexWriteLocker(RWMutex& m) : m(&m) {m.EnterWrite();}
	~RWMutexWriteLocker() {m->LeaveWrite();}
};

void Sleep(int ms);







NAMESPACE_UPP_END


#endif
