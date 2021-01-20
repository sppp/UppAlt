#ifndef _CoreAlt_Math_h_
#define _CoreAlt_Math_h_

NAMESPACE_UPP_BEGIN


struct Complex : public std::complex<double>, Moveable<Complex> {
	typedef std::complex<double> cmx;

	Complex()                              {}
	Complex(double r) : cmx(r)             {}
	Complex(double r, double i) : cmx(r,i) {}
	Complex(const cmx& c) : cmx(c)         {}
	
	bool operator==(const Complex& c) const { return (const cmx&)(*this) == (const cmx&)c; }
	bool operator!=(const Complex& c) const { return (const cmx&)(*this) != (const cmx&)c; }
	
};


NAMESPACE_UPP_END

#endif
