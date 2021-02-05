//$ class TopWindow {

private:
	Sppp::Windows* wm = NULL;
	Sppp::CoreWindow* cw = NULL;
	int id;
	
public:
	void Init(Sppp::Windows* wm, Sppp::CoreWindow* cw, int id);
	
	Sppp::CoreWindow* GetWindow() const {return cw;}
	Sppp::Windows* GetWindows() const {return wm;}
	
	
//$ };
