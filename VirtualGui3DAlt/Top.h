//$ class TopWindow {

private:
	Sppp::CoreWindow* cw = NULL;
	int id;
	
public:
	void Init(Sppp::CoreWindow* cw, int id);
	
	Sppp::CoreWindow* GetWindow() const {return cw;}
	
	
//$ };
