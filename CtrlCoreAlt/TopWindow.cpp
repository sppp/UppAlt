#include "CtrlCore.h"


NAMESPACE_UPP

int TopWindow::Run(bool appmodal) {
	Ctrl::Invalidate();
	EventLoop(this);
	return GetExitCode();
	
	/*
	GuiLock __;
	if(!IsOpen())
		Open();
	if(!IsVisible()) Show();
	bool pinloop = inloop;
	int  pexitcode = exitcode;
	exitcode = Null;
	Vector<Ctrl *> es;
	if(appmodal)
		es = GetTopCtrls();
	else {
		Vector<Ctrl *> ws = GetTopCtrls();
		for(int i = 0; i < ws.GetCount(); i++)
			if(ws[i]->InLoop())
				es.Add(ws[i]);
		Ctrl *mw = GetMainWindow();
		if(mw) GatherWindowTree(mw, ws, es);
	}
	Vector< Ptr<Ctrl> > disabled = DisableCtrls(es, this);
	
	EventLoop(this);
	
	EnableCtrls(disabled);
	if(IsNull(exitcode)) {
		WhenClose();
		if(IsNull(exitcode))
			DefaultBreak();
	}
	int q = exitcode;
	inloop = pinloop;
	exitcode = pexitcode;
	
	return q;*/
}


END_UPP_NAMESPACE
