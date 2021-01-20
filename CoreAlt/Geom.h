#ifndef _Core_Geom_h_
#define _Core_Geom_h_

NAMESPACE_UPP_BEGIN



template <class T>
struct Point_ : Moveable<Point_<T>> {
	T x = 0, y = 0;

	Point_() {}

	Point_(T x, T y) : x(x), y(y) {}

	Point_(const Point_& pt) {
		*this = pt;
	}




	double Length() { return sqrt(x * x + y * y); }
	double Slope() { return y / x; }

	Point_ operator+(const Point_& b) const { return Point_(x + b.x, y + b.y); }

	Point_ operator-() const { return Point_(-x, -y); }

	Point_ operator-(const Point_& b) const { return Point_(x - b.x, y - b.y); }

	Point_ operator*(double k) const { return Point_(x * k, y * k); }

	Point_ operator/(double k) const { return Point_(x / k, y / k); }

	template <class V>
	Point_& operator=(const V& b) {
		x = b.x;
		y = b.y;
		return *this;
	}

	Point_& operator+=(const Point_& b) {
		x += b.x;
		y += b.y;
		return *this;
	}

	Point_& operator-=(const Point_& b) {
		x -= b.x;
		y -= b.y;
		return *this;
	}

	Point_& operator*=(const double k) {
		x *= k;
		y *= k;
		return *this;
	}

	Point_& operator/=(const double k) {
		x /= k;
		y /= k;
		return *this;
	}

	static void Dot(const Point_& a, const Point_& b, Point_& o) {
		//dot product: o = a dot b
		o.x = a.x * b.x;
		o.y = a.y * b.y;
	}

	Point_ DotProd(const Point_& b) const {
		//return dot product
		return Point(x * b.x, y * b.y);
	}
	
	
	String ToString() const {return "Point(" + IntStr(x) + ", " + IntStr(y) + ")";}
};


typedef Point_<int> Point;
typedef Point_<float> Pointf;
typedef Point_<double> Pointd;



template <class T>
double GetSignedArea(const T& a, const T& b, const T& c) {
	double area = 0;
	area += (a.x * b.y - b.x * a.y);
	area += (b.x * c.y - c.x * b.y);
	area += (c.x * a.y - a.x * c.y);
	return area * 0.5;
}


template <class T>
struct Size_ : Moveable<Size_<T>> {
	T cx = 0, cy = 0;

	Size_() {}
	Size_(const Size_& sz) { *this = sz; }
	Size_(int cx, int cy) : cx(cx), cy(cy) {}
	
	bool IsEmpty() {return cx == 0 && cy == 0;}
	bool IsPositive() {return cx > 0 && cy > 0;}
	
	void operator=(const Size_& sz) {
		cx = sz.cx;
		cy = sz.cy;
	}

	Size_& operator+=(Size_ p) { cx += p.cx; cy += p.cy; return *this; }
	Size_& operator+=(T t) { cx += t;    cy += t;    return *this; }
	Size_& operator-=(Size_ p) { cx -= p.cx; cy -= p.cy; return *this; }
	Size_& operator-=(T t) { cx -= t;    cy -= t;    return *this; }
	Size_& operator*=(Size_ p) { cx *= p.cx; cy *= p.cy; return *this; }
	Size_& operator*=(T t) { cx *= t;    cy *= t;    return *this; }
	Size_& operator/=(Size_ p) { cx /= p.cx; cy /= p.cy; return *this; }
	Size_& operator/=(T t) { cx /= t;    cy /= t;    return *this; }
	Size_& operator<<=(int sh) { cx <<= sh;   cy <<= sh;   return *this; }
	Size_& operator>>=(int sh) { cx >>= sh;   cy >>= sh;   return *this; }
	
	String ToString() const {return "Size(" + IntStr(cx) + ", " + IntStr(cy) + ")";}
};

template <class T>
bool operator==(const Size_<T>& a, const Size_<T>& b) {
	return a.cx == b.cx && a.cy == b.cy;
}

typedef Size_<int> Size;
typedef Size_<float> Sizef;
typedef Size_<double> Sized;



template <class T>
struct Rect_ : Moveable<Rect_<T>> {
	T top = 0, left = 0, bottom = 0, right = 0;

	typedef Point_<T>  Pt;
	typedef Size_<T>   Sz;

	Rect_() {}
	Rect_(const Rect_& r) { *this = r; }
	Rect_(const Size_<T>& sz) { right = sz.cx; bottom = sz.cy; }
	Rect_(T l, T t, T r, T b) : top(t), left(l), bottom(b), right(r) {}

	void operator=(const Rect_& src) {
		top = src.top;
		left = src.left;
		bottom = src.bottom;
		right = src.right;
	}
	//bool operator!=(const Rect_& src) { return !(*this == src); }

	Point TopLeft() const { return Point(left, top); }

	T Width() const { return right - left; }
	T Height() const { return bottom - top; }
	Sz GetSize() const { return Sz(Width(), Height()); }
	bool Contains(const Point_<T>& pt) {return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom;}
	
	void   InflateHorz(T dx) { left -= dx; right += dx; }
	void   InflateVert(T dy) { top -= dy; bottom += dy; }
	void   Inflate(T dx, T dy) { InflateHorz(dx); InflateVert(dy); }
	void   Inflate(Sz sz) { Inflate(sz.cx, sz.cy); }
	void   Inflate(T dxy) { Inflate(dxy, dxy); }
	void   Inflate(T l, T t, T r, T b) { left -= l; top -= t; right += r; bottom += b; }
	void   Inflate(const Rect_& r) { Inflate(r.left, r.top, r.right, r.bottom); }

	void   DeflateHorz(T dx) { InflateHorz(-dx); }
	void   DeflateVert(T dy) { InflateVert(-dy); }
	void   Deflate(T dx, T dy) { Inflate(-dx, -dy); }
	void   Deflate(Sz sz) { Inflate(-sz); }
	void   Deflate(T dxy) { Inflate(-dxy); }
	void   Deflate(T l, T t, T r, T b) { Inflate(-l, -t, -r, -b); }
	void   Deflate(const Rect_& r) { Deflate(r.left, r.top, r.right, r.bottom); }

};

template <class T>
bool operator==(const Rect_<T>& a, const Rect_<T>& b) {
	return
		a.top    == b.top &&
		a.left   == b.left &&
		a.bottom == b.bottom &&
		a.right  == b.right;
}

typedef Rect_<int> Rect;
typedef Rect_<double> Rectf;


inline Rect RectC(int x, int y, int w, int h) { return Rect(x, y, x + w, y + h); }




template <class T>
T GetOffsets(const T& a, const T& b, double width) {
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	double len = sqrt(dx * dx + dy * dy);
	double scale = width / (2 * len);
	return T(-scale * dy, scale * dx);
}







Pointf Intersect(const Pointf& a, const Pointf& b, const Pointf& c, const Pointf& d);


NAMESPACE_UPP_END

#endif
