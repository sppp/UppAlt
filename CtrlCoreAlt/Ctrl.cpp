#include "CtrlCore.h"

NAMESPACE_UPP


void CtrlFrame::FramePaint(Draw& draw, const Rect& r) {}
void CtrlFrame::FrameAdd(Ctrl& ctrl) {}
void CtrlFrame::FrameRemove() {}
int CtrlFrame::OverPaint() const { return 0; }
void CtrlFrame::SetCapture() {
	if (!ctrl) return;
	SetCaptured(this);
	ctrl->SetCapture();
}

void CtrlFrame::ReleaseCapture() {
	if (!ctrl) return;
	SetCaptured(NULL);
	ctrl->ReleaseCapture();
}

CtrlFrame* CtrlFrame::GetCaptured() {
	TODO
	/*
	if (!ctrl) return NULL;
	return ctrl->GetWindows()->GetFrameCaptured();*/
}

CtrlFrame* CtrlFrame::GetWithMouse() {
	TODO
	/*
	if (!ctrl) return NULL;
	return ctrl->GetWindows()->GetFrameWithMouse();*/
}

void CtrlFrame::SetCaptured(CtrlFrame* c) {
	TODO
	/*
	ctrl->GetWindows()->SetFrameCaptured(c);*/
}

void CtrlFrame::SetWithMouse(CtrlFrame* c) {
	TODO
	/*
	ctrl->GetWindows()->SetFrameWithMouse(c);*/
}



bool Ctrl::do_debug_draw;




Ctrl::Ctrl() {
	cmd_begin.next = &cmd_frame;
	cmd_frame.prev = &cmd_begin;
	cmd_frame.next = &cmd_pre;
	cmd_pre.prev = &cmd_frame;
	cmd_pre.next = &cmd_post;
	cmd_post.prev = &cmd_pre;
	cmd_post.next = &cmd_end;
	cmd_end.prev = &cmd_post;
}

void Ctrl::Add(Ctrl& c) {
	for(int i = 0; i < children.GetCount(); i++)
		if (children[i] == &c)
			return;
	cmd_post.prev = &c.cmd_end;
	c.cmd_end.next = &cmd_post;
	if (children.GetCount()) {
		Ctrl& top = *children.Top();
		c.cmd_begin.prev = &top.cmd_end;
		top.cmd_end.next = &c.cmd_begin;
	}
	else {
		c.cmd_begin.prev = &cmd_pre;
		cmd_pre.next = &c.cmd_begin;
	}
	children.Add(&c);
	c.parent = this;
	c.Activate();
}

void Ctrl::AddChild(Ctrl* c) {
	if (!c) return;
	Add(*c);
}

Ctrl* Ctrl::GetLastChild() {
	return children.Top();
}

void Ctrl::RemoveChild(Ctrl* c) {
	for(int i = 0; i < children.GetCount(); i++) {
		if (children[i] == c) {
			/*DrawCommand* prev;
			if (i > 0)
				prev = &children[i-1]->cmd_end;
			else
				prev = &cmd_pre;
			
			DrawCommand* next;
			if (i < children.GetCount()-1)
				next = &children[i+1]->cmd_begin;
			else
				next = &cmd_post;*/
			
			c->cmd_end.next->prev = c->cmd_begin.prev;
			c->cmd_begin.prev->next = c->cmd_end.next;
			c->cmd_end.next = NULL;
			c->cmd_begin.prev = NULL;
			
			c->Deactivate();
			c->parent = NULL;
			children.Remove(i--);
		}
	}
}

Ctrl* Ctrl::GetParent() {
	return parent;
}

#if 0

CoreWindow* Ctrl::GetWindow() {
	Ctrl* c = this;
	while (c) {
		CoreWindow* cw = dynamic_cast<CoreWindow*>(c);
		if (cw)
			return cw;
		c = c->GetParent();
	}
	return NULL;
}

Windows* Ctrl::GetWindows() {
	Ctrl* c = this;
	while (c) {
		Windows* w = dynamic_cast<Windows*>(c);
		if (w)
			return w;
		c = c->GetParent();
	}
	return NULL;
}

#endif

bool Ctrl::IsShown() const {
	return !hidden;
}

void Ctrl::SetFrameRect(const Rect& r) {
	TODO
	/*
	this->frame_r = r;
	SetPendingEffectRedraw();
	if (parent) {
		parent->SetPendingLayout();
		CoreWindow* w = GetWindow();
		if (w)
			w->SetPendingLayout();
	}*/
}

void Ctrl::SetFocus() {
	TODO
	/*
	CoreWindow* w = GetWindow();
	if (w) w->DeepUnfocus();
	
	has_focus = true;
	has_focus_deep = true;
	GotFocus();
	
	Ctrl* wc = w;
	Ctrl* c = parent;
	while (c) {
		c->has_focus_deep = true;
		c->ChildGotFocus();
		if (c == w)
			break;
		c = c->GetParent();
	}*/
}


void Ctrl::Show(bool b) {
	hidden = !b;
}

void Ctrl::SetPendingRedrawDeep() {
	SetPendingEffectRedraw();
	SetPendingRedraw();
	for(int i = 0; i < children.GetCount(); i++) {
		children[i]->SetPendingRedrawDeep();
	}
}

void Ctrl::Refresh() {
	TODO
	/*SetPendingRedrawDeep();
	CoreWindow* win = GetWindow();
	if (win)
		win->SetPendingPartialRedraw();*/
}

void Ctrl::PostCallback(Callback cb) {
	SetTimeCallback(1, cb, this);
}

void Ctrl::PostRefresh() {
	PostCallback(THISBACK(Refresh0));
}

void Ctrl::PostLayoutCallback() {
	PostCallback(THISBACK(Layout0));
}

void Ctrl::SetCapture() {
	//SDL_CaptureMouse(SDL_TRUE);
	SetCaptured(this);
}

void Ctrl::ReleaseCapture() {
	//SDL_CaptureMouse(SDL_FALSE);
	SetCaptured(NULL);
}

void Ctrl::DeepUnfocus() {
	if (has_focus) {
		has_focus = false;
		has_focus_deep = false;
		LostFocus();
	}
	else if (has_focus_deep) {
		has_focus_deep = false;
		ChildLostFocus();
		
		for(int i = 0; i < children.GetCount(); i++) {
			Ctrl* c = children[i];
			if (c->HasFocusDeep()) {
				c->DeepUnfocus();
				break;
			}
		}
	}
}

Ctrl& Ctrl::HSizePos(int l, int r) {
	pos.htype = LogPos::HORZ;
	pos.l = l;
	pos.r = r;
	return *this;
}

Ctrl& Ctrl::VSizePos(int t, int b) {
	pos.vtype = LogPos::VERT;
	pos.t = t;
	pos.b = b;
	return *this;
}

Ctrl& Ctrl::BottomPos(int i, int size) {
	pos.vtype = LogPos::BOTTOM;
	pos.b = i;
	pos.h = size;
	return *this;
}

Ctrl& Ctrl::TopPos(int i, int size) {
	pos.vtype = LogPos::TOP;
	pos.t = i;
	pos.h = size;
	return *this;
}

Ctrl& Ctrl::LeftPos(int i, int size) {
	pos.htype = LogPos::LEFT;
	pos.l = i;
	pos.w = size;
	return *this;
}

Ctrl& Ctrl::RightPos(int i, int size) {
	pos.htype = LogPos::RIGHT;
	pos.r = i;
	pos.w = size;
	return *this;
}



bool Ctrl::Redraw(bool only_pending) {
	TODO
	/*
	Size sz(GetFrameSize());
	Rect new_content_r(sz);
	bool did_draw = false;
	
	if (pending_fx_redraw) {
		Draw fx(*cmd_begin.prev, cmd_begin, cmd_frame, *cmd_frame.next);
		
		fx.Offset(frame_r);
		
		fx.Link();
		
		pending_fx_redraw = false;
		did_draw = true;
	}
	
	
	if (pending_redraw) {
		Draw pre(*cmd_frame.prev, cmd_frame, cmd_pre, *cmd_pre.next);
		
		for(int i = 0; i < frames.GetCount(); i++) {
			CtrlFrame& f = *frames[i];
			f.FramePaint(pre, new_content_r);
			f.FrameLayout(new_content_r);
		}
		
		if (frames.GetCount())
			pre.Offset(new_content_r);
		
		Paint(pre);
		if (do_debug_draw) {
			if (has_mouse) {
				Rgba c{1, 0, 0, 1};
				c.a = 0.5;
				pre.DrawRect(new_content_r.GetSize(), c);
			}
			else {
				Rgba c(RandomColor(64, 128));
				c.a = 0.5;
				pre.DrawRect(new_content_r.GetSize(), c);
			}
		}
		
		pre.Link();
		
		content_r = new_content_r;
	}
	
	
	for(int i = 0; i < children.GetCount(); i++) {
		Ctrl* c = children[i];
		ASSERT(c);
		did_draw = c->Redraw(only_pending) || did_draw;
	}
	
	
	if (pending_redraw) {
		Draw post(*cmd_post.prev, cmd_post, cmd_end, *cmd_end.next);
		
		if (frames.GetCount())
			post.End();
		
		post.End();
		
		post.Link();
		
		did_draw = true;
		pending_redraw = false;
	}
	
	return did_draw;*/
}

Ctrl* Ctrl::GetCaptured() {
	TODO
	//return GetWindows()->GetCaptured();
}
Ctrl* Ctrl::GetWithMouse() {
	TODO
	//return GetWindows()->GetWithMouse();
}
void Ctrl::SetCaptured(Ctrl* c) {
	TODO
	//GetWindows()->SetCaptured(c);
}
void Ctrl::SetWithMouse(Ctrl* c) {
	TODO
	//GetWindows()->SetWithMouse(c);
}
CtrlFrame* Ctrl::GetFrameCaptured() {
	TODO
	//return GetWindows()->GetFrameCaptured();
}
CtrlFrame* Ctrl::GetFrameWithMouse() {
	TODO
	//return GetWindows()->GetFrameWithMouse();
}
void Ctrl::SetFrameCaptured(CtrlFrame* c) {
	TODO
	//GetWindows()->SetFrameCaptured(c);
}
void Ctrl::SetFrameWithMouse(CtrlFrame* c) {
	TODO
	//GetWindows()->SetFrameWithMouse(c);
}



void Ctrl::DeepLayout() {
	TODO
	/*Rect prev_frame_r = frame_r;
	
	const LogPos& lp = GetLogPos();
	if ((lp.htype || lp.vtype) && parent) {
		Rect cr = frame_r;
		Rect r(parent->content_r.GetSize());
		switch (lp.htype) {
			case LogPos::NO_HORZ:	break;
			case LogPos::LEFT:		cr.left = r.left + lp.l;		cr.right = cr.left + lp.w;		break;
			case LogPos::RIGHT:		cr.right = r.right - lp.r;		cr.left = cr.right - lp.w;		break;
			case LogPos::HORZ:		cr.left = r.left + lp.l;		cr.right = r.right - lp.r;		break;
		}
		switch (lp.vtype) {
			case LogPos::NO_VERT:	break;
			case LogPos::TOP:		cr.top = r.top + lp.t;			cr.bottom = cr.top + lp.h;		break;
			case LogPos::BOTTOM:	cr.bottom = r.bottom - lp.b;	cr.top = cr.bottom - lp.h;		break;
			case LogPos::VERT:		cr.top = r.top + lp.t;			cr.bottom = r.bottom - lp.b;	break;
		}
		SetFrameRect0(cr);
	}
	
	Size sz(frame_r.GetSize());
	Rect new_content_r(sz);
	for(int i = 0; i < frames.GetCount(); i++) {
		CtrlFrame& f = *frames[i];
		f.FrameLayout(new_content_r);
	}
	content_r = new_content_r;
	
	Layout();
	pending_layout = false;
	
	if (!(prev_frame_r == frame_r)) {
		SetPendingEffectRedraw();
		if (!(prev_frame_r.GetSize() == this->frame_r.GetSize()))
			SetPendingRedraw();
	}
	

	for(int i = 0; i < children.GetCount(); i++) {
		Ctrl& c = *children[i];
		c.DeepLayout();
	}
	
	
	PostLayout();*/
}

bool Ctrl::DeepKey(dword key, int count) {
	TODO
	/*if (HasFocus()) {
		return Key(key, count);
	}
	else {
		for(int i = 0; i < children.GetCount(); i++) {
			Ctrl& c = *children[i];
			if (c.HasFocusDeep())
				return c.DeepKey(key, count);
		}
		return false;
	}*/
}

bool Ctrl::DeepMouseMove(const Point& pt, dword keyflags) {
	TODO
	/*int deep_count = 0;
	for(int i = children.GetCount()-1; i >= 0; i--) {
		Ctrl* c = children[i];
		if (c->HasMouseDeep()) {
			if (deep_count)
				c->DeepMouseLeave();
			else
				deep_count++;
		}
	}
	ASSERT(deep_count <= 1);
	
	if (GetCaptured()) {
		if (has_mouse) {
			ASSERT(this == GetCaptured());
			Point ftl = frame_r.TopLeft();
			Point ctl = content_r.TopLeft();
			Point fpt = pt - ftl;
			Point cpt = fpt - ctl;
			
			MouseMove(cpt, keyflags);
			
			return true;
		}
		else if (has_mouse_deep) {
			Point ftl = frame_r.TopLeft();
			Point ctl = content_r.TopLeft();
			Point fpt = pt - ftl;
			Point cpt = fpt - ctl;
			
			if (GetFrameCaptured()) {
				Rect r(frame_r.GetSize());
				CtrlFrame* last_fitting = NULL;
				Rect fitting_rect;
				for(int i = 0; i < frames.GetCount(); i++) {
					CtrlFrame* f = frames[i];
					if (f == GetFrameCaptured()) {
						last_fitting = f;
						fitting_rect = r;
						break;
					}
					f->FrameLayout(r);
				}
				if (last_fitting) {
					CtrlFrame* f = last_fitting;
					Point fitting_pt = fpt - fitting_rect.TopLeft();
					f->MouseMove(fitting_pt, keyflags);
					return true;
				}
			}
			
			for(int i = children.GetCount()-1; i >= 0; i--) {
				Ctrl* c = children[i];
				if (c->has_mouse_deep)
					c->DeepMouseMove(cpt, keyflags);
			}
			return true;
		}
		return false;
	}
	else {
		#if 0
		{
			Point ftl = frame_r.TopLeft();
			Point fpt = pt - ftl;
			Point ctl = content_r.TopLeft();
			Point cpt = fpt - ctl;
			
			for(int i = children.GetCount()-1; i >= 0; i--) {
				Ctrl* c = children[i];
				if (dynamic_cast<Button*>(c) && c->frame_r.Contains(cpt)) {
					LOG("");
				}
			}
		}
		#endif
		if (frame_r.Contains(pt)) {
			has_mouse_deep = true;
			
			Point ftl = frame_r.TopLeft();
			Point fpt = pt - ftl;
			Point ctl = content_r.TopLeft();
			Point cpt = fpt - ctl;
			
			if (content_r.Contains(fpt)) {
				bool found = false;
				for(int i = children.GetCount()-1; i >= 0; i--) {
					Ctrl* c = children[i];
					if (found) {
						if (c->has_mouse_deep) {
							c->DeepMouseLeave();
							break;
						}
					}
					else if (c->DeepMouseMove(cpt, keyflags)) {
						found = true;
					}
				}
				
				if (!found) {
					CtrlFrame* frame_with_mouse = GetFrameWithMouse();
					if (frame_with_mouse) {
						frame_with_mouse->has_mouse = false;
						frame_with_mouse->MouseLeave();
						SetFrameWithMouse(NULL);
					}
					
					if (!has_mouse) {
						if (GetCaptured()) {ASSERT(GetCaptured() == this);}
						
						Ctrl* with_mouse = GetWithMouse();
						if (with_mouse) {
							with_mouse->has_mouse = false;
							with_mouse->MouseLeave();
						}
						has_mouse = true;
						
						SetWithMouse(this);
						
						MouseEnter(fpt, keyflags);
					}
					MouseMove(cpt, keyflags);
				}
			}
			else {
				Rect r(frame_r.GetSize());
				CtrlFrame* last_fitting = NULL;
				Rect fitting_rect;
				for(int i = 0; i < frames.GetCount(); i++) {
					CtrlFrame* f = frames[i];
					if (r.Contains(fpt)) {
						last_fitting = f;
						fitting_rect = r;
					}
					f->FrameLayout(r);
				}
				if (last_fitting) {
					Ctrl* with_mouse = GetWithMouse();
					if (with_mouse) {
						with_mouse->has_mouse = false;
						with_mouse->MouseLeave();
						SetWithMouse(NULL);
					}
					CtrlFrame* f = last_fitting;
					Point fitting_pt = fpt - fitting_rect.TopLeft();
					if (!f->has_mouse) {
						CtrlFrame* frame_with_mouse = GetFrameWithMouse();
						if (frame_with_mouse) {
							frame_with_mouse->has_mouse = false;
							frame_with_mouse->MouseLeave();
						}
						f->has_mouse = true;
						SetFrameWithMouse(f);
						
						f->MouseEnter(fitting_pt, keyflags);
					}
					f->MouseMove(fitting_pt, keyflags);
				}
			}
			
			return true;
		}
		else if (has_mouse_deep) {
			DeepMouseLeave();
		}
		else {
			ASSERT(!has_mouse);
		}
	}
	return false;*/
}

bool Ctrl::DeepMouse(int mouse_code, const Point& pt, dword keyflags) {
	TODO
	/*if (GetCaptured()) {
		if (has_mouse) {
			Point ftl = frame_r.TopLeft();
			Point ctl = content_r.TopLeft();
			Point fpt = pt - ftl;
			Point cpt = fpt - ctl;
			
			
			switch (mouse_code) {
				case LEFT_DOWN: LeftDown(cpt, keyflags); break;
				case LEFT_DOUBLE: LeftDouble(cpt, keyflags); break;
				case LEFT_TRIPLE: LeftTriple(cpt, keyflags); break;
				case LEFT_DRAG: LeftDrag(cpt, keyflags); break;
				case LEFT_HOLD: LeftHold(cpt, keyflags); break;
				case LEFT_REPEAT: LeftRepeat(cpt, keyflags); break;
				case LEFT_UP: LeftUp(cpt, keyflags); break;
				case RIGHT_DOWN: RightDown(cpt, keyflags); break;
				case RIGHT_DOUBLE: RightDouble(cpt, keyflags); break;
				case RIGHT_TRIPLE: RightTriple(cpt, keyflags); break;
				case RIGHT_DRAG: RightDrag(cpt, keyflags); break;
				case RIGHT_HOLD: RightHold(cpt, keyflags); break;
				case RIGHT_REPEAT: RightRepeat(cpt, keyflags); break;
				case RIGHT_UP: RightUp(cpt, keyflags); break;
				case MIDDLE_DOWN: MiddleDown(cpt, keyflags); break;
				case MIDDLE_DOUBLE: MiddleDouble(cpt, keyflags); break;
				case MIDDLE_TRIPLE: MiddleTriple(cpt, keyflags); break;
				case MIDDLE_DRAG: MiddleDrag(cpt, keyflags); break;
				case MIDDLE_HOLD: MiddleHold(cpt, keyflags); break;
				case MIDDLE_REPEAT: MiddleRepeat(cpt, keyflags); break;
				case MIDDLE_UP: MiddleUp(cpt, keyflags); break;
			}
			return true;
		}
		else if (has_mouse_deep) {
			Point ftl = frame_r.TopLeft();
			Point ctl = content_r.TopLeft();
			Point fpt = pt - ftl;
			Point cpt = fpt - ctl;
			
			CtrlFrame* frame_captured = GetFrameCaptured();
			if (frame_captured) {
				Rect r(frame_r.GetSize());
				CtrlFrame* last_fitting = NULL;
				Rect fitting_rect;
				for(int i = 0; i < frames.GetCount(); i++) {
					CtrlFrame* f = frames[i];
					if (f == frame_captured) {
						last_fitting = f;
						fitting_rect = r;
						break;
					}
					f->FrameLayout(r);
				}
				if (last_fitting) {
					CtrlFrame* f = last_fitting;
					Point fitting_pt = fpt - fitting_rect.TopLeft();
					switch (mouse_code) {
						case LEFT_DOWN: f->LeftDown(fitting_pt, keyflags); break;
						case LEFT_DOUBLE: f->LeftDouble(fitting_pt, keyflags); break;
						case LEFT_TRIPLE: f->LeftTriple(fitting_pt, keyflags); break;
						case LEFT_DRAG: f->LeftDrag(fitting_pt, keyflags); break;
						case LEFT_HOLD: f->LeftHold(fitting_pt, keyflags); break;
						case LEFT_REPEAT: f->LeftRepeat(fitting_pt, keyflags); break;
						case LEFT_UP: f->LeftUp(fitting_pt, keyflags); break;
						case RIGHT_DOWN: f->RightDown(fitting_pt, keyflags); break;
						case RIGHT_DOUBLE: f->RightDouble(fitting_pt, keyflags); break;
						case RIGHT_TRIPLE: f->RightTriple(fitting_pt, keyflags); break;
						case RIGHT_DRAG: f->RightDrag(fitting_pt, keyflags); break;
						case RIGHT_HOLD: f->RightHold(fitting_pt, keyflags); break;
						case RIGHT_REPEAT: f->RightRepeat(fitting_pt, keyflags); break;
						case RIGHT_UP: f->RightUp(fitting_pt, keyflags); break;
						case MIDDLE_DOWN: f->MiddleDown(fitting_pt, keyflags); break;
						case MIDDLE_DOUBLE: f->MiddleDouble(fitting_pt, keyflags); break;
						case MIDDLE_TRIPLE: f->MiddleTriple(fitting_pt, keyflags); break;
						case MIDDLE_DRAG: f->MiddleDrag(fitting_pt, keyflags); break;
						case MIDDLE_HOLD: f->MiddleHold(fitting_pt, keyflags); break;
						case MIDDLE_REPEAT: f->MiddleRepeat(fitting_pt, keyflags); break;
						case MIDDLE_UP: f->MiddleUp(fitting_pt, keyflags); break;
					}
					return true;
				}
			}
			
			for(int i = children.GetCount()-1; i >= 0; i--) {
				Ctrl* c = children[i];
				if (c->has_mouse_deep)
					c->DeepMouse(mouse_code, cpt, keyflags);
			}
			return true;
		}
		return false;
	}
	else {
		if (frame_r.Contains(pt)) {
			Point ftl = frame_r.TopLeft();
			Point ctl = content_r.TopLeft();
			Point fpt = pt - ftl;
			Point cpt = fpt - ctl;
			
			if (content_r.Contains(fpt)) {
				for(int i = children.GetCount()-1; i >= 0; i--) {
					Ctrl* c = children[i];
					if (c->DeepMouse(mouse_code, cpt, keyflags))
						return true;
				}
				switch (mouse_code) {
					case LEFT_DOWN: LeftDown(cpt, keyflags); break;
					case LEFT_DOUBLE: LeftDouble(cpt, keyflags); break;
					case LEFT_TRIPLE: LeftTriple(cpt, keyflags); break;
					case LEFT_DRAG: LeftDrag(cpt, keyflags); break;
					case LEFT_HOLD: LeftHold(cpt, keyflags); break;
					case LEFT_REPEAT: LeftRepeat(cpt, keyflags); break;
					case LEFT_UP: LeftUp(cpt, keyflags); break;
					case RIGHT_DOWN: RightDown(cpt, keyflags); break;
					case RIGHT_DOUBLE: RightDouble(cpt, keyflags); break;
					case RIGHT_TRIPLE: RightTriple(cpt, keyflags); break;
					case RIGHT_DRAG: RightDrag(cpt, keyflags); break;
					case RIGHT_HOLD: RightHold(cpt, keyflags); break;
					case RIGHT_REPEAT: RightRepeat(cpt, keyflags); break;
					case RIGHT_UP: RightUp(cpt, keyflags); break;
					case MIDDLE_DOWN: MiddleDown(cpt, keyflags); break;
					case MIDDLE_DOUBLE: MiddleDouble(cpt, keyflags); break;
					case MIDDLE_TRIPLE: MiddleTriple(cpt, keyflags); break;
					case MIDDLE_DRAG: MiddleDrag(cpt, keyflags); break;
					case MIDDLE_HOLD: MiddleHold(cpt, keyflags); break;
					case MIDDLE_REPEAT: MiddleRepeat(cpt, keyflags); break;
					case MIDDLE_UP: MiddleUp(cpt, keyflags); break;
				}
			}
			else {
				Rect r(frame_r.GetSize());
				CtrlFrame* last_fitting = NULL;
				Rect fitting_rect;
				for(int i = 0; i < frames.GetCount(); i++) {
					CtrlFrame* f = frames[i];
					if (r.Contains(fpt)) {
						last_fitting = f;
						fitting_rect = r;
					}
					f->FrameLayout(r);
				}
				if (last_fitting) {
					Ctrl* with_mouse = GetWithMouse();
					if (with_mouse) {
						with_mouse->has_mouse = false;
						with_mouse->MouseLeave();
						with_mouse = NULL;
						SetWithMouse(NULL);
					}
					CtrlFrame* f = last_fitting;
					Point fitting_pt = fpt - fitting_rect.TopLeft();
					if (!f->has_mouse) {
						ASSERT(!with_mouse);
						f->has_mouse = true;
						SetFrameWithMouse(f);
						
						f->MouseEnter(fitting_pt, keyflags);
					}
					switch (mouse_code) {
						case LEFT_DOWN: f->LeftDown(fitting_pt, keyflags); break;
						case LEFT_DOUBLE: f->LeftDouble(fitting_pt, keyflags); break;
						case LEFT_TRIPLE: f->LeftTriple(fitting_pt, keyflags); break;
						case LEFT_DRAG: f->LeftDrag(fitting_pt, keyflags); break;
						case LEFT_HOLD: f->LeftHold(fitting_pt, keyflags); break;
						case LEFT_REPEAT: f->LeftRepeat(fitting_pt, keyflags); break;
						case LEFT_UP: f->LeftUp(fitting_pt, keyflags); break;
						case RIGHT_DOWN: f->RightDown(fitting_pt, keyflags); break;
						case RIGHT_DOUBLE: f->RightDouble(fitting_pt, keyflags); break;
						case RIGHT_TRIPLE: f->RightTriple(fitting_pt, keyflags); break;
						case RIGHT_DRAG: f->RightDrag(fitting_pt, keyflags); break;
						case RIGHT_HOLD: f->RightHold(fitting_pt, keyflags); break;
						case RIGHT_REPEAT: f->RightRepeat(fitting_pt, keyflags); break;
						case RIGHT_UP: f->RightUp(fitting_pt, keyflags); break;
						case MIDDLE_DOWN: f->MiddleDown(fitting_pt, keyflags); break;
						case MIDDLE_DOUBLE: f->MiddleDouble(fitting_pt, keyflags); break;
						case MIDDLE_TRIPLE: f->MiddleTriple(fitting_pt, keyflags); break;
						case MIDDLE_DRAG: f->MiddleDrag(fitting_pt, keyflags); break;
						case MIDDLE_HOLD: f->MiddleHold(fitting_pt, keyflags); break;
						case MIDDLE_REPEAT: f->MiddleRepeat(fitting_pt, keyflags); break;
						case MIDDLE_UP: f->MiddleUp(fitting_pt, keyflags); break;
					}
				}
			}
			
			return true;
		}
		return false;
	}*/
}

bool Ctrl::DeepMouseWheel(const Point& pt, int zdelta, dword keyflags) {
	TODO
	/*if (GetCaptured()) {
		if (has_mouse) {
			ASSERT(this == GetCaptured());
			Point ftl = frame_r.TopLeft();
			Point ctl = content_r.TopLeft();
			Point cpt = pt - ftl - ctl;
			
			MouseWheel(cpt, zdelta, keyflags);
			return true;
		}
		else if (has_mouse_deep) {
			Point ftl = frame_r.TopLeft();
			Point ctl = content_r.TopLeft();
			Point cpt = pt - ftl - ctl;
			
			for(int i = children.GetCount()-1; i >= 0; i--) {
				Ctrl* c = children[i];
				if (c->has_mouse_deep)
					c->DeepMouseWheel(cpt, zdelta, keyflags);
			}
			
			return true;
		}
		return false;
	}
	else {
		if (frame_r.Contains(pt)) {
			Point ftl = frame_r.TopLeft();
			Point ctl = content_r.TopLeft();
			Point fpt = pt - ftl;
			Point cpt = fpt - ctl;
			
			if (content_r.Contains(fpt)) {
				for(int i = children.GetCount()-1; i >= 0; i--) {
					Ctrl* c = children[i];
					if (c->DeepMouseWheel(cpt, zdelta, keyflags))
						return true;
				}
				MouseWheel(cpt, zdelta, keyflags);
			}
			else {
				Rect r(frame_r.GetSize());
				CtrlFrame* last_fitting = NULL;
				Rect fitting_rect;
				for(int i = 0; i < frames.GetCount(); i++) {
					CtrlFrame* f = frames[i];
					if (r.Contains(fpt)) {
						last_fitting = f;
						fitting_rect = r;
					}
					f->FrameLayout(r);
				}
				if (last_fitting) {
					Ctrl* with_mouse = GetWithMouse();
					if (with_mouse) {
						with_mouse->has_mouse = false;
						with_mouse->MouseLeave();
						with_mouse = NULL;
						SetWithMouse(NULL);
					}
					CtrlFrame* f = last_fitting;
					Point fitting_pt = fpt - fitting_rect.TopLeft();
					if (!f->has_mouse) {
						CtrlFrame* frame_with_mouse = GetFrameWithMouse();
						if (frame_with_mouse) {
							frame_with_mouse->has_mouse = false;
							frame_with_mouse->MouseLeave();
						}
						f->has_mouse = true;
						SetFrameWithMouse(f);
						
						f->MouseEnter(fitting_pt, keyflags);
					}
					f->MouseWheel(fitting_pt, zdelta, keyflags);
				}
			}
			return true;
		}
		return false;
	}*/
}

void Ctrl::DeepMouseLeave() {
	TODO
	/*if (GetCaptured()) {
		
	}
	else {
		has_mouse_deep = false;
		if (has_mouse) {
			MouseLeave();
			SetWithMouse(NULL);
			has_mouse = false;
			
			CtrlFrame* frame_with_mouse = GetFrameWithMouse();
			if (frame_with_mouse) {
				frame_with_mouse->has_mouse = false;
				frame_with_mouse->MouseLeave();
				SetFrameWithMouse(NULL);
			}
		}
		for(int i = 0; i < children.GetCount(); i++) {
			Ctrl* c = children[i];
			if (c->has_mouse_deep) {
				c->DeepMouseLeave();
			}
		}
	}*/
}

END_UPP_NAMESPACE
