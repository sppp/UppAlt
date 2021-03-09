#ifndef _VirtualSound_VirtualSound_h_
#define _VirtualSound_VirtualSound_h_


#include <Sppp/Sppp.h>


NAMESPACE_UPP


class SystemSound : public Sppp::SoundProxy {
public:
	
	
};

struct VirtualSound {
	virtual SystemSound&	BeginPlay() = 0;
	virtual void			CommitPlay() = 0;
	virtual int				GetSampleRate() = 0;
	virtual void			UndoPlay() = 0;
	
};

extern VirtualSound* VirtualSoundPtr;


//#define SOUNDPLATFORM_CTRL_TOP_DECLS   Ctrl *owner_window;
//#define SOUNDPLATFORM_CTRL_DECLS_INCLUDE <VirtualSoundAlt/Ctrl.h>
//#define SOUNDPLATFORM_TOPWINDOW_DECLS_INCLUDE <VirtualSoundAlt/Top.h>
//#define SOUNDPLATFORM_INCLUDE_AFTER <VirtualSoundAlt/After.h>
//#define SOUNDPLATFORM_INCLUDE_AFTER_ECSLIB <VirtualSoundAlt/AfterEcsLib.h>


END_UPP_NAMESPACE




NAMESPACE_UPP



END_UPP_NAMESPACE

#endif
