#ifndef _DrawAlt_Font_h_
#define _DrawAlt_Font_h_


NAMESPACE_UPP


class Font {
	struct FontRef {
		TTF_Font *font = NULL;
		int refs = 1;
		
		String dir;
		String name;
		int ptsize = 0;
		int weight = 1;
		bool italic = false;
		
		~FontRef() {if (font) TTF_CloseFont(font); font = NULL;}
		void Inc() {refs++;}
		void Dec() {refs--; if (refs <= 0) delete this;}
	};
	
	FontRef* ref = NULL;
	
public:
	typedef Font CLASSNAME;
	Font() {}
	Font(const Font& fnt) {*this = fnt;}
	Font(Font&& fnt) {ref = fnt.ref; fnt.ref = NULL;}
	Font(TTF_Font* f) {if (f) {ref = new FontRef(); ref->font = f;}}
	Font(FontRef* r) {if (r) {ref = r;}}
	~Font() {Clear();}
	void operator=(const Font& fnt) {Clear(); ref = fnt.ref; if (ref) ref->Inc();}
	void Clear() {if (ref) ref->Dec(); ref = NULL;}
	bool IsEmpty() const {return ref == NULL;}
	TTF_Font* GetTTF_Font() {if (!ref) return NULL; return ref->font;}
	
	void SetScreenCloseClear();
	
	static FontRef* LoadFont(String dir, String name, int ptsize=16, int weight=1, bool italic=false);
};

Font StdFont(int size=15);

Size GetTextSize(String s, Font fnt);


END_UPP_NAMESPACE


#endif
