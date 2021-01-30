#include <EcsLib/EcsLib.h>


NAMESPACE_SPPP_BEGIN


/*#define IMAGECLASS WindowsImg
#define IMAGEFILE <WindowCtrl/Windows.iml>
#include <Draw/iml_source.h>*/

Image& WindowsImg::close() {
	static Image img;
	if (img.IsEmpty())
		img = StreamRaster::LoadFileAny(FindLocalFile("imgs" DIR_SEPS "close.png"));
	return img;
}

Image& WindowsImg::maximize() {
	static Image img;
	if (img.IsEmpty())
		img = StreamRaster::LoadFileAny(FindLocalFile("imgs" DIR_SEPS "maximize.png"));
	return img;
}

Image& WindowsImg::minimize() {
	static Image img;
	if (img.IsEmpty())
		img = StreamRaster::LoadFileAny(FindLocalFile("imgs" DIR_SEPS "minimize.png"));
	return img;
}

Image& WindowsImg::nwse() {
	static Image img;
	if (img.IsEmpty()) {
		img = StreamRaster::LoadFileAny(FindLocalFile("imgs" DIR_SEPS "nwse.png"));
		img.CenterHotSpot();
	}
	return img;
}

Image& WindowsImg::nesw() {
	static Image img;
	if (img.IsEmpty()) {
		img = StreamRaster::LoadFileAny(FindLocalFile("imgs" DIR_SEPS "nesw.png"));
		img.CenterHotSpot();
	}
	return img;
}

Image& WindowsImg::ns() {
	static Image img;
	if (img.IsEmpty()) {
		img = StreamRaster::LoadFileAny(FindLocalFile("imgs" DIR_SEPS "ns.png"));
		img.CenterHotSpot();
	}
	return img;
}

Image& WindowsImg::ew() {
	static Image img;
	if (img.IsEmpty()) {
		img = StreamRaster::LoadFileAny(FindLocalFile("imgs" DIR_SEPS "ew.png"));
		img.CenterHotSpot();
	}
	return img;
}


Windows::Windows() {
	win_counter = 0;
	maximize_all = false;
	active_pos = -1;
	active_id = -1;
	
	
	/*menu.WhenFocus = THISBACK(FocusWindowPos);
	menu.WhenClose = THISBACK(CloseWindow);
	menu.WhenMaximize = THISBACK(MaximizeWindow);
	menu.WhenMinimize = THISBACK(MinimizeWindow);
	menu.WhenTileWindows = THISBACK(OrderTileWindows);
	menu.WhenCloseAll = THISBACK(CloseAll);
	menu.WhenCloseOthers = THISBACK(CloseOthers);*/
}

Windows::~Windows() {
	
}

void Windows::AddWindow(CoreWindow& sw) {
	int id = win_counter++;
	int pos = wins.GetCount();
	wins.Add(id, &sw);
	Add(sw);
	sw.SetContent(this, id);
	int i = wins.GetCount();
	sw.SetFrameRect(RectC(i * 30, i * 30, 320, 240));
	if (maximize_all) MaximizeWindow(id);
	int prev_active_id = active_id;
	active_pos = pos;
	active_id = id;
	if (wins.Find(prev_active_id) != -1)
		wins.Get(prev_active_id)->Refresh();
	sw.tw->TopWindow::Init(this, &sw, id);
	WhenActiveWindowChanges();
	String title = sw.tw->GetTitle();
	sw.Title(title.GetCount() ? title : "Unnamed");
	Refresh();
	sw.Refresh();
}

CoreWindow& Windows::GetWindow(TopWindow& tw) {
	for(int i = 0; i < wins.GetCount(); i++) {
		TopWindow* ptr = wins[i]->GetTopWindow();
		if (ptr == &tw) {
			return *wins[i];
		}
	}
	NEVER();
}

void Windows::FocusWindow(TopWindow* tw) {
	for(int i = 0; i < wins.GetCount(); i++) {
		TopWindow* ptr = wins[i]->GetTopWindow();
		if (ptr == tw) {
			FocusWindow(i);
			return;
		}
	}
}

void Windows::MoveWindow(Point pt, int win_id) {
	if (maximize_all) return;
	CoreWindow& sw = *wins.Get(win_id);
	Size sz(sw.GetFrameSize());
	Point tl = sw.GetFrameRect().TopLeft();
	sw.SetFrameRect(RectC(tl.x + pt.x, tl.y + pt.y, sz.cx, sz.cy));
}

void Windows::FocusWindow(int win_id) {
	int i = wins.Find(win_id);
	if (i >= 0)
		FocusWindowPos(i);
}

void Windows::FocusWindowPos(int win_pos) {
	if (win_pos < 0 || win_pos >= wins.GetCount()) return;
	CoreWindow& sw = *wins[win_pos];
	sw.Show();
	Ctrl* c = &sw;
	if (GetLastChild() == c) {
		int prev_active_id = active_id;
		active_pos = win_pos;
		active_id = wins.GetKey(win_pos);
		if (wins.Find(prev_active_id) != -1)
			wins.Get(prev_active_id)->Refresh();
		Refresh();
		return;
	}
	if (!sw.IsMaximized()) {
		if (maximize_all) MaximizeWindow(wins.GetKey(win_pos));
	} else {
		Size sz(GetFrameSize());
		CoreWindow& sw = *wins[win_pos];
		sw.SetFrameRect(RectC(0, 0, sz.cx, sz.cy));
	}
	sw.GetTopWindow()->FocusEvent();
	RemoveChild(&sw);
	AddChild(&sw);
	int prev_active_id = active_id;
	active_pos = win_pos;
	active_id = wins.GetKey(win_pos);
	if (wins.Find(prev_active_id) != -1)
		wins.Get(prev_active_id)->Refresh();
	sw.Refresh();
	Refresh();
	WhenActiveWindowChanges();
}

void Windows::CloseWindow(int win_id) {
	int win_pos = wins.Find(win_id);
	if (win_pos == -1) return;
	CoreWindow& cw = *wins.Get(win_id);
	if (cw.HasMouseDeep())
		cw.DeepMouseLeave();
	if (cw.HasFocusDeep())
		cw.DeepUnfocus();
	RemoveChild(&cw);
	TopWindow* tw = cw.GetTopWindow();
	wins.Remove(win_pos);
	Refresh();
	if (tw) {
		bool found = false;
		for(int i = 0; i < wins.GetCount(); i++) {
			if (wins[i]->tw.Get() == tw) {
				wins.Remove(i);
				tw->CloseWindow();
				found = true;
				break;
			}
		}
		if (!found)
			tw->CloseWindow();
	}
	
	CloseWindow(cw);
	FocusPrevious();
	WhenWindowClose();
}

void Windows::MaximizeWindow(int win_id) {
	int win_pos = wins.Find(win_id);
	if (win_pos == -1) return;
	CoreWindow& sw = *wins[win_pos];
	SetWindowMaximized(sw, 1);
}

void Windows::RestoreWindow(int win_id) {
	int win_pos = wins.Find(win_id);
	if (win_pos == -1) return;
	CoreWindow& sw = *wins[win_pos];
	SetWindowMaximized(sw, 0);
}

void Windows::SetWindowMaximized(CoreWindow& sw, bool b) {
	Size sz(GetFrameSize());
	if (b) {
		if (!sw.IsMaximized()) {
			sw.StoreRect();
			sw.SetMaximized(true);
		}
		sw.SetFrameRect(RectC(0, 0, sz.cx, sz.cy));
		maximize_all = true;
	}
	else {
		sw.LoadRect();
		sw.SetMaximized(false);
		maximize_all = false;
		LoadRectAll();
		
		Size sw_size = sw.GetFrameSize();
		if (sw_size.cx > sz.cx ||sw_size.cy > sz.cy) {
			sw.SetFrameRect(RectC(10, 10, sz.cx-20, sz.cy-20));
		}
	}
}

void Windows::LoadRectAll() {
	for(int i = 0; i < wins.GetCount(); i++) {
		CoreWindow& sw = *wins[i];
		if (sw.IsMaximized()) {
			sw.LoadRect();
			sw.SetMaximized(false);
		}
	}
}

void Windows::MinimizeWindow(int win_id) {
	int win_pos = wins.Find(win_id);
	if (win_pos == -1) return;
	CoreWindow& sw = *wins[win_pos];
	sw.Hide();
	FocusPrevious();
}

void Windows::FocusPrevious() {
	int win_pos = active_pos;
	// Focus previous
	int count = wins.GetCount();
	while (win_pos >= count) win_pos--;
	for(int i = 0; i < count; i++) {
		if (win_pos <= -1) win_pos = count - 1;
		CoreWindow& sw2 = *wins[win_pos];
		//if (!maximize_all && !sw2.IsShown()) {
		if (!sw2.IsShown()) {
			win_pos--;
			continue;
		}
		FocusWindowPos(win_pos);
		return;
	}
	active_id = -1;
	active_pos = -1;
}

void Windows::SetTitle(int win_id, const String& title) {
	int win_pos = wins.Find(win_id);
	if (win_pos == -1) return;
	CoreWindow& sw = *wins[win_pos];
	sw.Title(title);
	PostRefresh();
}

String Windows::GetTitle(int win_id) {
	int win_pos = wins.Find(win_id);
	if (win_pos == -1) return "";
	CoreWindow& sw = *wins[win_pos];
	return sw.GetTitle();
}

void Windows::CloseAll() {
	for(int i = 0; i < wins.GetCount(); i++) {
		QueueCloseWindow(wins.GetKey(i));
	}
}

void Windows::CloseOthers(int win_id) {
	for (int i = 0; i < wins.GetCount(); i++) {
		if (wins.GetKey(i) == win_id)
			;
		else
			QueueCloseWindow(wins.GetKey(i));
	}
}

bool Windows::Key(dword key, int count) {
	if (key == (K_CTRL|K_F4)) {
		QueueCloseWindow(active_id);
		return true;
	}
	if (key == (K_CTRL|K_TAB)) {
		active_pos++;
		if (active_pos >= wins.GetCount()) active_pos = 0;
		FocusWindowPos(active_pos);
		return true;
	}
	if (key == (K_CTRL|K_SHIFT|K_TAB)) {
		active_pos--;
		if (active_pos < 0) active_pos = wins.GetCount()-1;
		FocusWindowPos(active_pos);
		return true;
	}
	return false;
}

void Windows::LeftDown(Point p, dword keyflags) {
	//LOG(p);
	
}

void Windows::Layout() {
	// Resize active Window if it is maximised and Windows area size changes (TopWindow area changes usually)
	if (maximize_all) {
		if (active_pos < 0 || active_pos >= wins.GetCount())
			return;
		Size sz(GetFrameSize());
		CoreWindow& sw = *wins[active_pos];
		sw.SetFrameRect(RectC(0, 0, sz.cx, sz.cy));
	}
}

void Windows::PostLayout() {
	CoreWindow* win = GetActiveWindow();
	if (win && win->IsMaximized())
		SetWindowMaximized(*win, true);
}




TopWindow* Windows::GetVisibleTopWindow() {
	Ctrl* last = GetLastChild();
	for(int i = 0; i < wins.GetCount(); i++) {
		CoreWindow* swc = wins[i];
		Ctrl* ptr = swc;
		if (ptr == last) {
			return swc->GetTopWindow();
		}
	}
	return 0;
}

void Windows::OrderTileWindows() {
	int count = wins.GetCount();
	if (maximize_all && count == 1) return;
	
	Size sz(GetFrameSize());
	
	double target_ratio = 16.0 / 9.0; //4.0 / 3.0;
	
	int rows, cols, mod, diff;
	int smallest_diff = INT_MAX;
	int smallest_diff_cols = -1;
	double smallest_diff_ratio_diff = DBL_MAX, ratio, ratio_diff;
	for(int i = 1; i < 100; i++) {
		rows = count / i;
		if (rows == 0) break;
		cols = i;
		mod = count % (rows * cols);
		diff = abs(rows-cols);
		//if (mod == 0)
			ratio = ((double)sz.cx/cols) / ((double)sz.cy/rows);
		//else
			//ratio = (((double)sz.cx/cols) / ((double)sz.cy/rows) * (rows * (cols-1)) + ((double)sz.cx/cols) / ((double)sz.cy/(rows + mod)) * ((rows + mod) * 1)) / count;
		ratio_diff = fabs(ratio - target_ratio);
		
		//LOG(Format("rows=%d cols=%d mod=%d diff=%d ratio_diff=%f", rows, cols, mod, diff, ratio_diff));
		
		//if (i <= smallest_diff) {
		if (ratio_diff < smallest_diff_ratio_diff && mod <= 2) {
			smallest_diff = diff;
			smallest_diff_cols = i;
			smallest_diff_ratio_diff = ratio_diff;
		}
	}
	rows = count / smallest_diff_cols;
	if (rows == 0) return;
	cols = smallest_diff_cols;
	mod = count % (rows * cols);
	diff = abs(rows-cols);
	ratio_diff = smallest_diff_ratio_diff;
	
	//LOG(Format("rows=%d cols=%d mod=%d diff=%d ratio_diff=%f", rows, cols, mod, diff, ratio_diff));
	
	
	double x_step, y_step, ymod_step;
	x_step = (double)sz.cx / cols;
	y_step = (double)sz.cy / rows;
	ymod_step = (double)sz.cy / (rows + mod);
	
	maximize_all = false;
	
	int pos = 0;
	for(int i = 0; i < cols; i++) {
		int x = i * x_step;
		int rows_i;
		double yi_step;
		if (i < cols-1) {
			rows_i = rows;
			yi_step = y_step;
		} else {
			rows_i = rows + mod;
			yi_step = ymod_step;
		}
		for(int j = 0; j < rows_i; j++) {
			int y = j * yi_step;
			CoreWindow& sw = *wins[pos];
			sw.SetFrameRect(RectC(x, y, x_step, yi_step));
			sw.SetMaximized(false);
			pos++;
		}
	}
}

void Windows::OrderTileWindowsVert() {
	int count = wins.GetCount();
	if (maximize_all && count == 1) return;
	
	Size sz(GetFrameSize());
	
	double y_step = (double)sz.cy / wins.GetCount();
	
	maximize_all = false;
	
	for(int i = 0; i < wins.GetCount(); i++) {
		CoreWindow& sw = *wins[i];
		sw.SetFrameRect(RectC(0, i * y_step, sz.cx, y_step));
		sw.SetMaximized(false);
	}
}

bool Windows::CheckRender() {
	bool do_render = false;
	
	if (IsPendingLayout()) {
		DeepLayout();
		SetPendingEffectRedraw();
	}
	
	for(int i = 0; i < GetCount(); i++) {
		Ctrl* c = Ctrl::operator[](i);
		
		if (c->IsPendingLayout()) {
			c->DeepLayout();
			c->SetPendingEffectRedraw();
		}
	}
	
	do_render = Redraw(true) || do_render;
	
	return do_render;
}

bool Windows::DeepKey(dword key, int count) {
	if (wins.GetCount()) {
		CoreWindow& win = *wins.Top();
		return win.DeepKey(key, count);
	}
	return false;
}

/*void Windows::Redraw(int child_id, bool only_pending) {
	DrawCommand* prev;
	if (child_id == 0) {
		prev = &cmd_begin;
	}
	else {
		prev = &sub_area[child_id-1]->GetEndCommand();
	}
	
	DrawCommand* next;
	if (child_id == sub_area.GetCount()-1) {
		next = &cmd_end;
	}
	else {
		next = &sub_area[child_id+1]->GetBeginCommand();
	}
	
	
	Ctrl& c = *sub_area[child_id];
	//Draw pre(*prev, c.GetBeginCommand(), c.GetEndCommand(), *next);
	
	d.Offset(c.GetRect());
	c.Redraw(d, only_pending);
	d.End();
	
	d.Link();
}*/


NAMESPACE_SPPP_END
