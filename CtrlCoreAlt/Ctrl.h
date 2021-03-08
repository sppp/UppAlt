#ifndef _CtrlCoreAlt_Ctrl_h_
#define _CtrlCoreAlt_Ctrl_h_

namespace Sppp {

class Windows;
class WindowManager;
class CoreWindow;

}


NAMESPACE_UPP

class Ctrl;

enum {
	EVENT_INVALID,
	EVENT_WINDOW_RESIZE,
	EVENT_SHUTDOWN,
	EVENT_KEYDOWN,
	EVENT_KEYUP,
	EVENT_MOUSEMOVE,
	//EVENT_MOUSEDRAG,
	EVENT_MOUSEWHEEL,
	EVENT_MOUSE_EVENT,
};

inline String GetEventTypeString(int event) {
	switch (event) {
		case EVENT_WINDOW_RESIZE:	return "Window Resize";
		case EVENT_SHUTDOWN:		return "Shutdown";
		case EVENT_KEYDOWN:			return "Key Down";
		case EVENT_KEYUP:			return "Key Up";
		case EVENT_MOUSEMOVE:		return "Mouse Move";
		//case EVENT_MOUSEDRAG:		return "Mouse Drag";
		case EVENT_MOUSEWHEEL:		return "Mouse Wheel";
		case EVENT_MOUSE_EVENT:		return "Mouse Event";
		case EVENT_INVALID:
		default:					return "<invalid>";
	}
}

struct CtrlEvent {
	int type = 0;
	dword value = 0;
	int n = 0;
	Point pt;
	Size sz;
	
	void Clear() {
		type = 0;
		value = 0;
		n = 0;
		pt = Point(0,0);
		sz = Size(0,0);
	}
};

class CtrlFrame {
	
protected:
	friend class Ctrl;
	
	Ctrl* ctrl = NULL;
	bool has_mouse = false;
	
	CtrlFrame* GetCaptured();
	CtrlFrame* GetWithMouse();
	void SetCaptured(CtrlFrame* c);
	void SetWithMouse(CtrlFrame* c);
	
public:
	virtual void FrameLayout(Rect& r) = 0;
	//virtual void FrameAddSize(Size& sz) = 0;
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAdd(Ctrl& parent);
	virtual void FrameRemove();
	virtual int  OverPaint() const;
	virtual void MouseEnter(Point frame_p, dword keyflags) {}
	virtual void MouseMove(Point frame_p, dword keyflags) {}
	virtual void MouseLeave() {}
	virtual void LeftDown(Point p, dword keyflags) {}
	virtual void LeftDouble(Point p, dword keyflags) {}
	virtual void LeftTriple(Point p, dword keyflags) {}
	virtual void LeftDrag(Point p, dword keyflags) {}
	virtual void LeftHold(Point p, dword keyflags) {}
	virtual void LeftRepeat(Point p, dword keyflags) {}
	virtual void LeftUp(Point p, dword keyflags) {}
	virtual void RightDown(Point p, dword keyflags) {}
	virtual void RightDouble(Point p, dword keyflags) {}
	virtual void RightTriple(Point p, dword keyflags) {}
	virtual void RightDrag(Point p, dword keyflags) {}
	virtual void RightHold(Point p, dword keyflags) {}
	virtual void RightRepeat(Point p, dword keyflags) {}
	virtual void RightUp(Point p, dword keyflags) {}
	virtual void MiddleDown(Point p, dword keyflags) {}
	virtual void MiddleDouble(Point p, dword keyflags) {}
	virtual void MiddleTriple(Point p, dword keyflags) {}
	virtual void MiddleDrag(Point p, dword keyflags) {}
	virtual void MiddleHold(Point p, dword keyflags) {}
	virtual void MiddleRepeat(Point p, dword keyflags) {}
	virtual void MiddleUp(Point p, dword keyflags) {}
	virtual void MouseWheel(Point p, int zdelta, dword keyflags) {}
	virtual void ContinueGlobalMouseMomentum() {}
	void SetCapture();
	void ReleaseCapture();

	CtrlFrame() {}
	virtual ~CtrlFrame() {}

private:
	CtrlFrame(const CtrlFrame&);
	void operator=(const CtrlFrame&);
};


struct KeyInfo {
	const char *name;
	dword key[4];
};


class Bar {
public:
	struct Item {
		virtual Item& Text(const char *text);
		virtual Item& Key(dword key);
		virtual Item& Repeat(bool repeat = true);
		virtual Item& Image(const class Image& img);
		virtual Item& Check(bool check);
		virtual Item& Radio(bool check);
		virtual Item& Enable(bool _enable = true);
		virtual Item& Bold(bool bold = true);
		virtual Item& Tip(const char *tip);
		virtual Item& Help(const char *help);
		virtual Item& Topic(const char *topic);
		virtual Item& Description(const char *desc);
		virtual void  FinalSync();

		Item&   Label(const char *text);
		Item&   RightLabel(const char *text);

		Item& Key(KeyInfo& (*key)());

		Item();
		virtual ~Item();
	};
	
private:
	
	
	
public:
	typedef Bar CLASSNAME;
	Bar();
	
	Bar& Add(String title, Callback cb);
	Bar& Separator();
	
};


struct LogPos {
	
	enum {
		NO_HORZ = 0,
		LEFT,
		RIGHT,
		HORZ,
		NO_VERT = 0,
		TOP,
		BOTTOM,
		VERT
	};
	int l = 0, r = 0, t = 0, b = 0, w = 0, h = 0;
	Byte vtype = 0, htype = 0;
};

class TopWindow;
//class CoreWindow;

class Ctrl : public Pte<Ctrl> {
	
public:
	enum {
		UNKNOWN,
		LEFT_DOWN,
		LEFT_DOUBLE,
		LEFT_TRIPLE,
		LEFT_DRAG,
		LEFT_HOLD,
		LEFT_REPEAT,
		LEFT_UP,
		RIGHT_DOWN,
		RIGHT_DOUBLE,
		RIGHT_TRIPLE,
		RIGHT_DRAG,
		RIGHT_HOLD,
		RIGHT_REPEAT,
		RIGHT_UP,
		MIDDLE_DOWN,
		MIDDLE_DOUBLE,
		MIDDLE_TRIPLE,
		MIDDLE_DRAG,
		MIDDLE_HOLD,
		MIDDLE_REPEAT,
		MIDDLE_UP,
	};
	
protected:
	
	
	
	
	static  int       LoopLevel;
	static  Ctrl     *LoopCtrl;
	static  int64     EventLoopNo;
	static  bool      do_debug_draw;
	
	Ctrl* GetCaptured();
	Ctrl* GetWithMouse();
	void SetCaptured(Ctrl* c);
	void SetWithMouse(Ctrl* c);
	CtrlFrame* GetFrameCaptured();
	CtrlFrame* GetFrameWithMouse();
	void SetFrameCaptured(CtrlFrame* c);
	void SetFrameWithMouse(CtrlFrame* c);
	
	
	
	bool         inloop:1;
	bool         ignore_mouse:1;
	bool         hidden:1;
	bool         want_focus:1;
	bool         has_focus:1;
	bool         has_focus_deep:1;
	bool         has_mouse:1;
	bool         has_mouse_deep:1;
	bool         pending_fx_redraw:1;
	bool         pending_redraw:1;
	bool         pending_layout:1;
	
	Ctrl* parent = NULL;
	Vector<Ctrl*> children;
	Vector<CtrlFrame*> frames;
	LogPos pos;
	Rect frame_r, content_r;
	DrawCommand cmd_begin, cmd_frame, cmd_pre, cmd_post, cmd_end;
	
	virtual bool Redraw(bool only_pending);
	bool DeepKey(dword key, int count);
	bool DeepMouseMove(const Point& pt, dword keyflags);
	bool DeepMouse(int mouse_code, const Point& pt, dword keyflags);
	bool DeepMouseWheel(const Point& pt, int zdelta, dword keyflags);
	void DeepMouseLeave();
	void Refresh0() {Refresh();}
	void Layout0() {Layout();}
	
protected:
	friend struct SDL2GUI3DAlt;
	
	void SetFrameRect0(const Rect& r) {this->frame_r = r;}
	
public:
	typedef Ctrl CLASSNAME;
	Ctrl();
	virtual ~Ctrl() {}
	
	static void SetDebugDraw(bool b=true) {do_debug_draw = b;}
	static void CloseTopCtrls();
	static bool ProcessEvent(bool *quit = NULL);
	static bool ProcessEvents(bool *quit = NULL);
	static bool  ReleaseCtrlCapture();
	static Ctrl *GetCaptureCtrl();
	static Image OverrideCursor(const Image& m);
	static Image DefaultCursor();
	
	//void EventLoop(Ctrl *ctrl);
	
	void Add(Ctrl& c);
	void AddFrame(CtrlFrame& c) {c.ctrl = this; frames.Add(&c); SetPendingRedraw();}
	void AddChild(Ctrl* c);
	Ctrl* GetLastChild();
	void RemoveChild(Ctrl* c);
	void DeepLayout();
	
	Ctrl* GetParent();
	TopWindow* GetTopWindow();
	int GetCount() const {return children.GetCount();}
	Ctrl* operator[](int i) {return children[i];}
	
	Rect GetFrameRect() const {return frame_r;}
	Rect GetContentRect() const {return content_r;}
	Rect GetRect() const {return GetFrameRect();}
	Rect GetWorkArea() const;
	Size GetFrameSize() const {return frame_r.GetSize();}
	Size GetContentSize() const {return content_r.GetSize();}
	bool IsShown() const;
	bool HasFocus() const {return has_focus;}
	bool HasFocusDeep() const {return has_focus_deep;}
	bool HasMouse() const {return has_mouse;}
	bool HasMouseDeep() const {return has_mouse_deep;}
	bool IsPendingLayout() const {return pending_layout;}
	const LogPos& GetLogPos() const {return pos;}
	Size GetSize() const {return GetContentSize();}
	
	virtual void SetFrameRect(const Rect& r);
	void SetFrameRect(int x, int y, int w, int h) {SetFrameRect(Rect(x, y, x+w, y+h));}
	void SetRect(const Rect& r) {SetFrameRect(r);}
	void SetContentRect(const Rect& r) {content_r = r;}
	void SetPendingLayout() {pending_layout = true;}
	void SetPendingRedraw() {pending_redraw = true;}
	void SetPendingRedrawDeep();
	void SetPendingEffectRedraw() {pending_fx_redraw = true;}
	void SetFocus();
	void WantFocus(bool b=true) {want_focus = b;}
	void IgnoreMouse(bool b=true) {ignore_mouse = b;}
	
	void Show(bool b=true);
	void Hide() {Show(false);}
	void Refresh();
	void PostCallback(Callback cb);
	void PostRefresh();
	void PostLayoutCallback();
	void SetCapture();
	void ReleaseCapture();
	void DeepUnfocus();
	
	Ctrl& SizePos() {return HSizePos().VSizePos();}
	Ctrl& HSizePos(int l=0, int r=0);
	Ctrl& VSizePos(int t=0, int b=0);
	Ctrl& BottomPos(int i, int size);
	Ctrl& TopPos(int i, int size);
	Ctrl& LeftPos(int i, int size);
	Ctrl& RightPos(int i, int size);
	
	virtual void Activate() {}
	virtual void Deactivate() {}
	virtual Image FrameMouseEvent(int event, Point p, int zdelta, dword keyflags) {return DefaultImages::Arrow;}
	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags) {return DefaultImages::Arrow;}
	virtual void MouseEnter(Point frame_p, dword keyflags) {if (do_debug_draw) Refresh();}
	virtual void MouseMove(Point content_p, dword keyflags) {}
	virtual void LeftDown(Point p, dword keyflags) {}
	virtual void LeftDouble(Point p, dword keyflags) {}
	virtual void LeftTriple(Point p, dword keyflags) {}
	virtual void LeftDrag(Point p, dword keyflags) {}
	virtual void LeftHold(Point p, dword keyflags) {}
	virtual void LeftRepeat(Point p, dword keyflags) {}
	virtual void LeftUp(Point p, dword keyflags) {}
	virtual void RightDown(Point p, dword keyflags) {}
	virtual void RightDouble(Point p, dword keyflags) {}
	virtual void RightTriple(Point p, dword keyflags) {}
	virtual void RightDrag(Point p, dword keyflags) {}
	virtual void RightHold(Point p, dword keyflags) {}
	virtual void RightRepeat(Point p, dword keyflags) {}
	virtual void RightUp(Point p, dword keyflags) {}
	virtual void MiddleDown(Point p, dword keyflags) {}
	virtual void MiddleDouble(Point p, dword keyflags) {}
	virtual void MiddleTriple(Point p, dword keyflags) {}
	virtual void MiddleDrag(Point p, dword keyflags) {}
	virtual void MiddleHold(Point p, dword keyflags) {}
	virtual void MiddleRepeat(Point p, dword keyflags) {}
	virtual void MiddleUp(Point p, dword keyflags) {}
	virtual void MouseWheel(Point p, int zdelta, dword keyflags) {}
	virtual void MouseLeave() {if (do_debug_draw) Refresh();}
	virtual void PadTouch(int controller, Pointf p) {}
	virtual void PadUntouch(int controller) {}
	virtual Image CursorImage(Point p, dword keyflags) {return DefaultImages::Arrow;}
	virtual bool Key(dword key, int count) {return false;}
	virtual void Paint(Draw& d) {}
	virtual void Layout() {}
	virtual void PostLayout() {}
	virtual void GotFocus() {}
	virtual void LostFocus() {}
	virtual void ChildGotFocus() {}
	virtual void ChildLostFocus() {}
	
	Callback WhenAction;
	
	
	
	Ctrl& operator <<= (Callback cb) {WhenAction = cb; return *this;}
	
	
	
	
	
	
	
	enum PlacementConstants {
		CENTER   = 0,
		MIDDLE   = 0,
		LEFT     = 1,
		RIGHT    = 2,
		TOP      = 1,
		BOTTOM   = 2,
		SIZE     = 3,

		MINSIZE  = -16380,
		MAXSIZE  = -16381,
		STDSIZE  = -16382,
	};
	
	enum MouseEvents {
		BUTTON        = 0x0F,
		ACTION        = 0xF0,

		MOUSEENTER    = 0x10,
		MOUSEMOVE     = 0x20,
		MOUSELEAVE    = 0x30,
		CURSORIMAGE   = 0x40,
		MOUSEWHEEL    = 0x50,

		DOWN          = 0x80,
		UP            = 0x90,
		DOUBLE        = 0xa0,
		REPEAT        = 0xb0,
		DRAG          = 0xc0,
		HOLD          = 0xd0,
		TRIPLE        = 0xe0,

		LEFTDOWN      = LEFT|DOWN,
		LEFTDOUBLE    = LEFT|DOUBLE,
		LEFTREPEAT    = LEFT|REPEAT,
		LEFTUP        = LEFT|UP,
		LEFTDRAG      = LEFT|DRAG,
		LEFTHOLD      = LEFT|HOLD,
		LEFTTRIPLE    = LEFT|TRIPLE,

		RIGHTDOWN     = RIGHT|DOWN,
		RIGHTDOUBLE   = RIGHT|DOUBLE,
		RIGHTREPEAT   = RIGHT|REPEAT,
		RIGHTUP       = RIGHT|UP,
		RIGHTDRAG     = RIGHT|DRAG,
		RIGHTHOLD     = RIGHT|HOLD,
		RIGHTTRIPLE   = RIGHT|TRIPLE,

		MIDDLEDOWN     = MIDDLE|DOWN,
		MIDDLEDOUBLE   = MIDDLE|DOUBLE,
		MIDDLEREPEAT   = MIDDLE|REPEAT,
		MIDDLEUP       = MIDDLE|UP,
		MIDDLEDRAG     = MIDDLE|DRAG,
		MIDDLEHOLD     = MIDDLE|HOLD,
		MIDDLETRIPLE   = MIDDLE|TRIPLE
	};
	
	
	#include GUIPLATFORM_CTRL_DECLS_INCLUDE
	
	
	
	
};

class EmptySpaceCtrl : public Ctrl {
	
};




/*class Screen : public Ctrl {
	
	
public:
	virtual ~Screen() {}
	
	virtual bool Init() = 0;
	virtual void AddWindow(CoreWindow&) = 0;
	virtual bool Poll(ScreenEvent& e) = 0;
	virtual void Render() = 0;
	virtual void Shutdown() = 0;
	virtual void ProcessCloseQueue() = 0;
	
};*/


class ParentCtrl : public Ctrl {
	
};






END_UPP_NAMESPACE

#endif
