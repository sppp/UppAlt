#include "Core.h"


NAMESPACE_UPP_BEGIN

void IdParser::Parse() {
	const String& n = input;
	int& cursor = pos;
	
	if (cursor < input.GetCount()) {
		int chr = input[cursor];
		
		if (IsAlpha(chr) || chr == '_') {
			id.Cat(chr);
			cursor++;
			while (cursor < input.GetCount()) {
				int chr = input[cursor];
				if (IsAlpha(chr) || chr == '_' || IsDigit(chr)) {
					id.Cat(chr);
					cursor++;
				}
				else
					break;
			}
			is_valid = true;
		}
	}
}


void StringParser::Parse() {
	const String& n = input;
	int& cursor = pos;
	
	if (cursor < input.GetCount()) {
		int chr = input[cursor];
		if (chr == '\"') {
			cursor++;
			while (cursor < input.GetCount()) {
				int chr = input[cursor++];
				if (chr == '\"') {
					is_valid = true;
					break;
				}
				str.Cat(chr);
			}
		}
	}
}


void NumberParser::Parse() {
	int& cursor = pos;
	String& n = str;
	
	bool neg_value = false;
	int chr = input[cursor];
	if (chr == '-') {
		n.Cat(chr);
		if (++cursor >= input.GetCount()) chr = 0;
		else chr = input[cursor];
		neg_value = true;
	}
	else if (chr == '+') {
		n.Cat(chr);
		if (++cursor >= input.GetCount()) chr = 0;
		else chr = input[cursor];
	}
	if (IsDigit(chr)) {
		int upcoming_type = 0;
		bool is_float = false;
		n.Cat(chr);
		cursor++;
		enum {INT, DEC, FRAC, SIGN, EXP, F, END, OCTHEX, OCT, HEX};
		int exp = chr == '0' ? OCTHEX : INT;
		while (cursor < input.GetCount()) {
			int chr = input[cursor];
			bool cat = false;
			
			if (exp == INT) {
				if (IsDigit(chr)) {
					cat = true;
					upcoming_type = NumberParser::INT;
				}
				else if (chr == '.') {
					exp = FRAC;
					cat = true;
					upcoming_type = is_float ? NumberParser::FLOAT : NumberParser::DOUBLE;
				}
				else if (chr == 'e' || chr == 'E') {
					exp = SIGN;
					cat = true;
					upcoming_type = is_float ? NumberParser::FLOAT : NumberParser::DOUBLE;
				}
			}
			else if (exp == FRAC) {
				if (IsDigit(chr))
					cat = true;
				else if (chr == 'e' || chr == 'E') {
					exp = SIGN;
					cat = true;
					upcoming_type = is_float ? NumberParser::FLOAT : NumberParser::DOUBLE;
				}
			}
			else if (exp == SIGN) {
				if (chr == '+' || chr == '-' || IsDigit(chr)) {
					exp = EXP;
					cat = true;
				}
				else if (chr == 'f' || chr == 'F') {
					exp = END;
					cat = true;
					is_float = true;
					upcoming_type = NumberParser::FLOAT;
				}
			}
			else if (exp == EXP) {
				if (IsDigit(chr))
					cat = true;
				else if (chr == 'f' || chr == 'F') {
					exp = END;
					cat = true;
					is_float = true;
					upcoming_type = NumberParser::FLOAT;
				}
			}
			else if (exp == OCTHEX) {
				if (chr == 'x' || chr == 'X') {
					exp = HEX;
					cat = true;
					upcoming_type = NumberParser::HEX;
				}
				else if (chr >= '0' && chr <= '7') {
					exp = OCT;
					cat = true;
					upcoming_type = NumberParser::OCT;
				}
				else if (chr == '.') {
					exp = FRAC;
					cat = true;
					upcoming_type = is_float ? upcoming_type = NumberParser::FLOAT : NumberParser::DOUBLE;
				}
			}
			else if (exp == HEX) {
				if ((chr >= '0' && chr <= '9') ||
					(chr >= 'a' && chr <= 'f') ||
					(chr >= 'A' && chr <= 'F')) {
					cat = true;
				}
			}
			else if (exp == OCT) {
				if (chr >= '0' && chr <= '7') {
					cat = true;
				}
			}
			
			if (cat) {
				cursor++;
				n.Cat(chr);
				if (exp == END) break;
			}
			else break;
		}
		
		
		this->type = upcoming_type;
		if (upcoming_type == NumberParser::OCT) {
			i32 = (neg_value ? -1 : +1) * OctInt(n);
			u32 = i32;
			i64 = i32;
			f = i32;
			d = i32;
		}
		else if (upcoming_type == NumberParser::HEX) {
			i32 = (neg_value ? -1 : +1) * HexInt(n);
			u32 = i32;
			i64 = i32;
			f = i32;
			d = i32;
		}
		else if (upcoming_type == NumberParser::INT) {
			i64 = (neg_value ? -1 : +1) * StrInt64(n);
			u32 = i64;
			i32 = i64;
			f = i64;
			d = i64;
		}
		else if (upcoming_type == NumberParser::FLOAT || upcoming_type == NumberParser::DOUBLE) {
			d = (neg_value ? -1 : +1) * StrDbl(n);
			i32 = d;
			u32 = d;
			i64 = d;
			f = d;
		}
	}
}








void CParser::SkipSpaces() {
	while (!IsEnd()) {
		if (!IsSpace(input[pos]))
			break;
		pos++;
	}
}

bool CParser::IsEnd() const {
	return pos >= input.GetCount();
}

/*FileLocation CParser::GetLocation() const {
	FileLocation loc;
	loc.col = 1;
	loc.line = 1;
	loc.file = "CParser";
	const char* it = input.Begin();
	const char* end = it + std::max(0, pos);
	while (it != end) {
		char chr = *it++;
		if (chr == '\n') {
			loc.col = 1;
			loc.line++;
		}
		else {
			loc.col++;
		}
	}
	return loc;
}*/

bool CParser::GetId(String& str) {
	IdParser ip(input, pos);
	if (ip.is_valid) {
		pos = ip.pos;
		str = ip.id;
		DoSpaces();
		return true;
	}
	return false;
}

bool CParser::GetInt(int64& i64) {
	NumberParser np(input, pos);
	if (np.type == NumberParser::INT ||
		np.type == NumberParser::HEX ||
		np.type == NumberParser::OCT) {
		pos = np.pos;
		i64 = np.i64;
		DoSpaces();
		return true;
	}
	return false;
}

bool CParser::GetFloating(double& dbl) {
	NumberParser np(input, pos);
	if (np.type == NumberParser::FLOAT || np.type == NumberParser::DOUBLE) {
		pos = np.pos;
		dbl = np.d;
		DoSpaces();
		return true;
	}
	return false;
}

String CParser::ReadId() {
	IdParser ip(input, pos);
	if (ip.is_valid) {
		pos = ip.pos;
		DoSpaces();
		return ip.id;
	}
	throw Exc("Couldn't read id");
}

String CParser::ReadString() {
	StringParser ip(input, pos);
	if (ip.is_valid) {
		pos = ip.pos;
		DoSpaces();
		return ip.str;
	}
	throw Exc("Couldn't read string");
}

int CParser::ReadInt() {
	NumberParser np(input, pos);
	if (np.type == NumberParser::INT ||
		np.type == NumberParser::HEX ||
		np.type == NumberParser::OCT) {
		pos = np.pos;
		DoSpaces();
		return np.i32;
	}
	throw Exc("Couldn't read int");
}

double CParser::ReadNumber() {
	NumberParser np(input, pos);
	if (np.type != NumberParser::INVALID) {
		pos = np.pos;
		DoSpaces();
		return np.d;
	}
	throw Exc("Couldn't read number");
}

void CParser::PassChar(char chr) {
	if (pos < input.GetCount() && input[pos] == chr) {
		pos++;
		DoSpaces();
	}
	else {
		String s; s.Cat(chr);
		throw Exc("Char didn't match: " + s);
	}
}

void CParser::PassNewline() {
	PassChar('\n');
}

bool CParser::Id(String id) {
	IdParser ip(input, pos);
	if (ip.is_valid && ip.id == id) {
		pos = ip.pos;
		DoSpaces();
		return true;
	}
	return false;
}

bool CParser::Char(char chr) {
	if (IsChar(chr)) {
		pos++;
		DoSpaces();
		return true;
	}
	return false;
}

bool CParser::Newline() {
	return Char('\n');
}

bool CParser::IsId() const {
	IdParser ip(input, pos);
	return ip.is_valid;
}

bool CParser::IsId(String id) const {
	IdParser ip(input, pos);
	if (ip.is_valid && ip.id == id) {
		return true;
	}
	return false;
}

bool CParser::IsInt() const {
	NumberParser np(input, pos);
	if (np.type == NumberParser::INT ||
		np.type == NumberParser::HEX ||
		np.type == NumberParser::OCT) {
		return true;
	}
	return false;
}

bool CParser::IsChar(char i) const {
	if (pos < input.GetCount())
		return input[pos] == i;
	return false;
}

bool CParser::IsCharDigit() const {
	if (pos < input.GetCount())
		return IsDigit(input[pos]);
	return false;
}

bool CParser::IsNumber() const {
	NumberParser np(input, pos);
	return np.type!= NumberParser::INVALID;
}

bool CParser::IsString() const {
	StringParser ip(input, pos);
	return ip.is_valid;
}






NAMESPACE_UPP_END
