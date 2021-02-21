#ifndef _Core_Tuple_h_
#define _Core_Tuple_h_

NAMESPACE_UPP_BEGIN

template<std::size_t N, typename T, typename... types>
struct GetNthType
{
    using type = typename GetNthType<N - 1, types...>::type;
};

template<typename T, typename... types>
struct GetNthType<0, T, types...>
{
    using type = T;
};

template<std::size_t N, typename... Args>
using GetType = typename GetNthType<N, Args...>::type;





template<typename First, typename... Rest>
struct Tuple : Moveable<Tuple<First, Rest...>> {
	Tuple() {}
	Tuple(First a, Rest... b): b(b...), a(a) {}
	Tuple(First a, Tuple<Rest...> b): b(b), a(a) {}
	Tuple(const Tuple& t) {*this = t;}
	void operator=(const Tuple& t) {a = t.a; b = t.b;}
	
	First a;
	Tuple<Rest...> b;
	
	First& operator->() {return a;}
	template <class T> T* Find() const {if (typeid(&a) == typeid(T)) return (T*)&a; if (typeid(a) == typeid(T)) return (T*)&a; return b.template Find<T>();}
	template <class T> void ForEach(T fn) const {fn(a); b.ForEach(fn);}
};

template<typename First>
struct Tuple<First> : Moveable<Tuple<First>> {
	Tuple() {}
	Tuple(First a): a(a) {}
	Tuple(const Tuple& t) {*this = t;}
	void operator=(const Tuple& t) {a = t.a;}
	
	First a;
	
	First& operator->() {return a;}
	template <class T> T* Find() const {if (typeid(&a) == typeid(T)) return (T*)&a; if (typeid(a) == typeid(T)) return (T*)&a; return NULL;}
	template <class T> void ForEach(T fn) const {fn(a);}
	
	operator First& () {return a;}
	operator const First& () const {return a;}
	
};

template<int index, typename First, typename... Rest>
struct GetImpl {
	static auto value(const Tuple<First, Rest...>* t) -> decltype(GetImpl < index - 1, Rest... >::value(&t->b)) {
		return GetImpl < index - 1, Rest... >::value(&t->b);
	}
};

template<typename First, typename... Rest>
struct GetImpl<0, First, Rest...> {
	static First value(const Tuple<First, Rest...>* t) {
		return t->a;
	}
};

template<int index, typename First, typename... Rest>
auto Get(const Tuple<First, Rest...>& t) -> decltype(GetImpl<index, First, Rest...>::value(&t)) { //typename Type<index, First, Rest...>::value {
	return GetImpl<index, First, Rest...>::value(&t);
}

template <class T, class A> T Get(const A& a) {
	T* ptr = a.template Find<T>();
	if (!ptr) throw Exc("Couldn't find type from tuple");
	return *ptr;
}


template <class ... T>
Tuple<T...> MakeTuple(T... args) {
	return Tuple<T...>(args...);
}



/*template <class First, class T, class R>
Tuple<First, T::> TupleCat(First f, T b) {
	//return Tuple<First, decltype(b.b)>(f.a, b.b);
}*/

template <class A, class B>
struct RefPair : Moveable<RefPair<A,B>> {
	A& a;
	B& b;

	RefPair(A& a, B& b) : a(a), b(b) {}
	RefPair(const RefPair& r) : a(r.a), b(r.b) {}
	
	RefPair* operator->(){return this;} // hack to allow returning object from Iterator
};

template <class A, class B>
struct PtrPair : Moveable<PtrPair<A,B>> {
	A* a;
	B* b;

	PtrPair() : a(0), b(0) {}
	PtrPair(A* a, B* b) : a(a), b(b) {}
	PtrPair(const PtrPair& r) : a(r.a), b(r.b) {}
	
	PtrPair* operator->(){return this;} // hack to allow returning object from Iterator
};

NAMESPACE_UPP_END

#endif
