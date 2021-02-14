#ifndef _VirtualGuiAlt_VirtualGuiAlt_h_
#define _VirtualGuiAlt_VirtualGuiAlt_h_

#include <DrawAlt/Draw.h>


NAMESPACE_UPP

struct VirtualGui3DAlt;

enum {
	EVENT_INVALID,
	EVENT_SHUTDOWN,
};

struct CtrlEvent {
	int type = 0;
};

class SystemDraw : public DrawProxy {
public:
	VirtualGui3DAlt* gui = 0;
	
	Size GetPageSize() const override;
	void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color) override;
	void DrawRectOp(int x, int y, int cx, int cy, Color color) override;
	void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		             Color ink, int n, const int *dx) override;
	void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                        const int *counts, int count_count,
	                        int width, Color color, Color doxor) override;
	
	bool    CanSetSurface()                         { return false; }
	static void Flush()                             {}
};

enum KM {
	KM_NONE  = 0x00,

	KM_LSHIFT= 0x01,
	KM_RSHIFT= 0x02,
	KM_LCTRL = 0x04,
	KM_RCTRL = 0x08,
	KM_LALT  = 0x10,
	KM_RALT  = 0x20,

	KM_CAPS  = 0x40,
	KM_NUM   = 0x80,
	
	KM_CTRL = KM_LCTRL | KM_RCTRL,
	KM_SHIFT = KM_LSHIFT | KM_RSHIFT,
	KM_ALT = KM_LALT | KM_RALT,
};

struct VirtualGui3DAlt {
	virtual Size        GetSize() = 0;
	virtual dword       GetMouseButtons() = 0;
	virtual dword       GetModKeys() = 0;
	virtual bool        IsMouseIn() = 0;
	//virtual bool        ProcessEvent(bool *quit) = 0;
	virtual bool        Poll(Upp::CtrlEvent& e) = 0;
	virtual void        WaitEvent(int ms) = 0;
	virtual void        WakeUpGuiThread() = 0;
	virtual void        SetMouseCursor(const Image& image) = 0;
	virtual void        Quit() = 0;
	virtual bool        IsWaitingEvent() = 0;
	virtual SystemDraw& BeginDraw() = 0;
	virtual void        CommitDraw() = 0;
	virtual uint32      GetTickCount() = 0;
};

extern VirtualGui3DAlt* VirtualGui3DAltPtr;

class ImageDraw : public SImageDraw {
public:
	ImageDraw(Size sz) : SImageDraw(sz) {}
	ImageDraw(int cx, int cy) : SImageDraw(cx, cy) {}
};

#define GUIPLATFORM_CTRL_TOP_DECLS   Ctrl *owner_window;

#define GUIPLATFORM_CTRL_DECLS_INCLUDE <VirtualGui3DAlt/Ctrl.h>

#define GUIPLATFORM_PASTECLIP_DECLS \
	bool dnd; \
	friend struct DnDLoop; \

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <VirtualGui3DAlt/Top.h>

#define GUIPLATFORM_INCLUDE_AFTER <VirtualGui3DAlt/After.h>
#define GUIPLATFORM_INCLUDE_AFTER_ECSLIB <VirtualGui3DAlt/AfterEcsLib.h>

END_UPP_NAMESPACE




NAMESPACE_UPP



END_UPP_NAMESPACE

#endif
