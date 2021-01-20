#include "Core.h"


NAMESPACE_UPP_BEGIN

Value ParseMap(CParser& tk);



Value ParseArray(CParser& p) {
	String str;
	int64 i64;
	double dbl;
	ValueArray arr;
	
	p.PassChar('[');
	
	while (!p.IsChar(']')) {
		
		if (p.IsChar('{')) {
			arr.Add(ParseMap(p));
		}
		else if (p.IsChar('[')) {
			arr.Add(ParseArray(p));
		}
		else if (p.IsChar('\"')) {
			String value = p.ReadString();
			arr.Add(value);
		}
		else if (p.GetId(str)) {
			if (str == "true")
				arr.Add(true);
			else if (str == "false")
				arr.Add(false);
			else
				arr.Add(str);
		}
		else if (p.GetInt(i64)) {
			arr.Add(i64);
		}
		else if (p.GetFloating(dbl)) {
			arr.Add(dbl);
		}
		else {
			throw Exc("Unexpected token");
		}
		
		p.Char(',');
	}
	
	p.PassChar(']');
	
	return arr;
}

Value ParseMap(CParser& p) {
	ValueMap vm;
	
	p.PassChar('{');
	
	while (!p.IsChar('}')) {
		String key;
		if (p.IsId())
			key = p.ReadId();
		else
			key = p.ReadString();
		
		if (p.IsChar(',') || p.IsChar('}')) {
			p.Char(',');
			vm.Add(key);
			continue;
		}
		
		p.PassChar(':');
		
		if (p.IsChar('{')) {
			vm.Add(key, ParseMap(p));
		}
		else if (p.IsChar('[')) {
			vm.Add(key, ParseArray(p));
		}
		else if (p.IsId()) {
			String value = p.ReadId();
			if (value == "true")
				vm.Add(key, true);
			else if (value == "false")
				vm.Add(key, false);
			else
				vm.Add(key, value);
		}
		else if (p.IsInt()) {
			int64 value = p.ReadInt();
			vm.Add(key, value);
		}
		else if (p.IsNumber()) {
			double value = p.ReadNumber();
			vm.Add(key, value);
		}
		else if (p.IsString()) {
			String value = p.ReadString();
			vm.Add(key, value);
		}
		else {
			throw Exc("Unexpected token");
		}
		
		p.Char(',');
	}
	
	p.PassChar('}');
	
	return vm;
}

Value ParseJSON(String json) {
	Value v;
	CParser p(json);
	
	try {
		v = ParseMap(p);
	}
	catch (Exc e) {
		LOG("JSON parsing error: " << e);
	}
	
	return v;
}


NAMESPACE_UPP_END
