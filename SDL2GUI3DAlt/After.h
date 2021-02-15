#ifndef _SDL2GUI3DAlt_After_h_
#define _SDL2GUI3DAlt_After_h_

NAMESPACE_UPP
using namespace Sppp;

struct SDL2GUI3DAlt_MachineData {
	Shared<EntityStore> ents;
	Shared<Entity> cam;
	SoftRenderer sw_rend;
	SoftFramebufferDraw sw_draw;
	//HardRenderer rend;
	
	
};


END_UPP_NAMESPACE

#endif
