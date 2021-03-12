#include "OOSDL2.h"

NAMESPACE_SDL2_BEGIN


void Bundle::AddDefaultComponents() {
	GetAddComponent<Timer>();
	if (IsEnabled(AUDIO_IN))	GetAddComponent<AudioInput>();
	if (IsEnabled(AUDIO_OUT))	GetAddComponent<AudioOutput>();
	if (IsEnabled(SCREEN))		GetAddComponent<Screen>();
	if (IsEnabled(EVENTS))		GetAddComponent<Events>();
	if (IsEnabled(JOYSTICK))	GetAddComponent<Joystick>();
	if (IsEnabled(GAMECTRL))	GetAddComponent<GameController>();
	if (IsEnabled(SENSOR))		GetAddComponent<Sensor>();
	if (IsEnabled(IMAGE))		GetAddComponent<Image>();
	if (IsEnabled(FONT))		GetAddComponent<Font>();
}

bool Bundle::Open() {
	Context* ctx = this;
	
	if (comps.IsEmpty())
		AddDefaultComponents();
	
	
	for (Component& c : comps)
		Context::Add(c);
	
	return Context::Open();
}

void Bundle::Close() {
	return Context::Close();
}


NAMESPACE_SDL2_END
