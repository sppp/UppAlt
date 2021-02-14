#include <EcsLib/EcsLib.h>


NAMESPACE_SPPP_BEGIN

uint32 DirectWindow::prev_ticks;


DirectWindow::DirectWindow() {
	
}

void DirectWindow::Initialize() {
	uint32 flags = 0;
	
	Shared<RenderingSystem> sys = GetEntity().GetMachine().Get<RenderingSystem>();
	if (!sys)
		return;
	
	prev_ticks = VirtualGui3DAltPtr->GetTickCount();
	
	sys->AddRenderable(*this);
	
	is_open = true;
}

void DirectWindow::Update(float dt) {
	
	if (screen_output_cb) {
		SystemDraw& draw = VirtualGui3DAltPtr->BeginDraw();
		
		screen_output_cb->Render(draw);
		
		VirtualGui3DAltPtr->CommitDraw();
	}
	/*
	if (pipe || screen_output_cb) {
		SDL_RenderCopy(display_renderer, fb, NULL, NULL);
		SwapBuffer();
	}
	*/
}

void DirectWindow::Uninitialize() {
	if (is_open)
		WhenClose();
	
	/*if (display_renderer) {
		SDL_DestroyRenderer(display_renderer);
		display_renderer = 0;
	}
	
	if (display_window) {
		SDL_DestroyWindow(display_window);
		display_window = 0;
	}
    
    if (fb) {
        SDL_DestroyTexture(fb);
        fb = 0;
    }*/
    
    Shared<RenderingSystem> sys = GetEntity().GetMachine().Get<RenderingSystem>();
	if (sys)
		sys->RemoveRenderable(*this);
	
	is_open = false;
}

bool DirectWindow::Attach(ScreenOutput& output) {
	auto type = output.GetType();
	
	if (type == PIPELINE_CPU) {
		Shared<Pipeline> pipe = output.GetPipeline();
		if (pipe) {
			// trash
			//auto& fb_shader = pipe->Create<SurfaceFbShader>("graphics.pixel");
			//fb_shader.SetFramebuffer(draw);
			
			
			TODO
			/*if (!output.SetFramebuffer(draw)) {
				LOG("error: ScreenSDL2: couldn't set screen framebuffer");
				return false;
			}
			this->pipe = pipe;*/
			return true;
		}
	}
	else if (type == RENDERER_FB) {
		/*if (!output.SetFramebuffer(draw)) {
			LOG("error: ScreenSDL2: couldn't set screen framebuffer");
			return false;
		}*/
		screen_output_cb = &output;
		return true;
	}
	else Panic("Unimplemented");
	
	return false;
}

bool DirectWindow::Attach(SoundOutput& output)  {
	TODO
}



NAMESPACE_SPPP_END