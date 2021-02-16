#ifndef _SDL2GUI3DAlt_After_h_
#define _SDL2GUI3DAlt_After_h_

NAMESPACE_UPP
using namespace Sppp;

struct SDL2GUI3DAlt_MachineData {
	Shared<EntityStore> ents;
	Shared<Entity> cam;
	SoftRenderer sw_rend;
	SoftFramebufferDraw sw_draw;
	HardRenderer hw_rend;
	HardFramebufferDraw hw_draw;
	
	
};


END_UPP_NAMESPACE

#endif
