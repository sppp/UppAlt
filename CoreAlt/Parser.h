#ifndef _Core_Parser_h_
#define _Core_Parser_h_

NAMESPACE_UPP_BEGIN
	

struct IdParser {
	bool is_valid = false;
	const String& input;
	int pos;
	String id;
	
	IdParser(const String& input, int pos) : input(input), pos(pos) {Parse();}
	
	void Parse();
	
};

struct StringParser {
	bool is_valid = false;
	const String& input;
	int pos;
	String str;
	
	StringParser(const String& input, int pos) : input(input), pos(pos) {Parse();}
	
	void Parse();
	
};

struct NumberParser {
	enum {
		INVALID,
		OCT,
		HEX,
		INT,
		FLOAT,
		DOUBLE
	};
	
	int type = 0;
	const String& input;
	int pos;
	String str;
	uint32 u32;
	int32 i32;
	int64 i64;
	float f;
	double d;
	
	NumberParser(const String& input, int pos) : input(input), pos(pos) {Parse();}
	
	void Parse();
	
};

class CParser {
	String input;
	int pos = 0;
	
	bool pass_whitespace = true;
	void DoSpaces() {if (pass_whitespace) SkipSpaces();}
	void SkipSpaces();
public:
	typedef CParser CLASSNAME;
	CParser() {}
	CParser(String s) {input = s;}
	
	
	void Next() {pos++;}
	bool IsEnd() const;
	//FileLocation GetLocation() const;
	
	String ReadId();
	String ReadString();
	int ReadInt();
	double ReadNumber();
	void PassNewline();
	void PassChar(char chr);
	
	bool Char(char chr);
	bool Id(String id);
	bool Newline();
	bool GetId(String& str);
	bool GetInt(int64& i64);
	bool GetFloating(double& dbl);
	
	bool IsId() const;
	bool IsId(String id) const;
	bool IsInt() const;
	bool IsChar(char i) const;
	bool IsCharDigit() const;
	bool IsNumber() const;
	bool IsString() const;
	
};

NAMESPACE_UPP_END

#endif
