#ifndef _DrawAlt_Image_h_
#define _DrawAlt_Image_h_


NAMESPACE_UPP_BEGIN


typedef enum {
	IMGDATA_NULL,
	IMGDATA_RAW,
	
	#ifdef flagSDL2
	IMGDATA_SDL2_SURFACE,
	#endif
} ImgDataType;

class Image : Moveable<Image> {
	
public:
	struct ImageDataRef {
		ImgDataType type = IMGDATA_NULL;
		void* obj = NULL;
		const byte* pixels = NULL;
		uint16 width = 0, height = 0, stride = 0, pitch = 0;
		Point hotspot;
		int refs = 0;
		ImageDataRef() : hotspot(0,0) {}
		~ImageDataRef() {Clear();}
		void Inc() {refs++;}
		void Dec() {refs--; if (refs <= 0) delete this;}
		void Clear();
	};
	
	
	Image() {}
	Image(const Image& img) {*this = img;}
	Image(Image&& img) {data = img.data; img.data = NULL;}
	Image(ImageDataRef* data) : data(data) {data->Inc();}
	//Image(SDL_Surface* s) {if (s) {data = new ImageDataRef(); data->data = s;}}
	~Image() {Clear();}
	void operator=(const Image& img) {Clear(); data = img.data; if (data) data->Inc();}
	void Clear() {if (data) data->Dec(); data = NULL;}
	bool IsEmpty() const {return data == NULL;}
	void SetHotSpot(Point pt) {if (data) data->hotspot = pt;}
	Point GetHotSpot() const {if (data) return data->hotspot; return Point(0,0);}
	void CenterHotSpot() {if (data) data->hotspot = Point(GetWidth()/2, GetHeight()/2);}
	ImageDataRef* GetData() const {return data;}
	//GLuint GetTexture() const {if (!data) return 0; return data->texture;}
	int GetWidth() const {if (!data) return 0; return data->width;}
	int GetHeight() const {if (!data) return 0; return data->height;}
	Size GetSize() const {if (!data) return Size(0,0); return Size(data->width, data->height);}
	const byte* GetIter(int x, int y) const;
	void Serialize(Stream& s) {TODO}
	operator bool() const {return data;}
	
	static Image Arrow();
	
	
private:
	ImageDataRef* data = NULL;
	
};

#ifdef flagSDL2
Image LoadAnyImage_SDL2(String path);
void FreeImageData_SDL2_Surface(void* v);
#endif




class ImageBuffer : Moveable<ImageBuffer> {
	Vector<RGBA> buf;
	Size sz;
	
public:
	ImageBuffer() : sz(0,0) {}
	ImageBuffer(Size sz) {Create(sz);}
	ImageBuffer(int w, int h) {Create(Size(w, h));}
	
	
	void Create(Size sz);
	void Clear();
	
	RGBA* Begin() {return buf.Begin();}
	RGBA* End() {return buf.End();}
	
	Image GetImage();
	
	operator Image() {return GetImage();}
};





class StreamRaster {
	
	
public:
	
	static Image LoadFileAny(String path);
	
};









struct FramebufferDraw {
	
	
	
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual byte* GetIterator(int x, int y) = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual int GetStride() const = 0;
	virtual int GetPitch() const = 0;
	
	int GetBufferSize() const {return GetHeight() * GetPitch();}
	
	void Zero();
	void DrawPixel(int x, int y, RGBA color);
	void DrawPixel(int x, int y, byte* color);
	void DrawHLine(int x0, int x1, int y, byte* color);
	void DrawLine(int x0, int y0, int x1, int y1, byte* color);
	void DrawLine(const Point& a, const Point& b, byte* color) {DrawLine(a.x, a.y, b.x, b.y, color);}
	void DrawLine(const Pointf& a, const Pointf& b, byte* color) {DrawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y, color);}
	
};

NAMESPACE_UPP_END


#endif
