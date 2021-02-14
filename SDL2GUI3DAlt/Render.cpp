#include <EcsLib/EcsLib.h>
#include "After.h"

NAMESPACE_UPP
using namespace Sppp;


void SDL2GUI3DAlt::Render(bool do_render) {
	if (data->ents.IsEmpty())
		data->ents = GetMachine().Get<EntityStore>();
	
	Vector<Tuple<Entity*, Transform2D*, CoreWindow*>> wins = data->ents->GetComponentsWithEntity<Transform2D, CoreWindow>();
	Windows* windows = Ctrl::GetWindows();
	
	for(int i = 0; i < wins.GetCount(); i++) {
		Tuple<Entity*, Transform2D*, CoreWindow*>& tuple = wins[i];
		Transform2D& t = *tuple.b.a;
		CoreWindow& cw = *tuple.b.b.a;
		
		if (cw.GetId() < 0)
			windows->AddWindow(cw);
		
		Rect r = cw.GetFrameRect();
		Size t_size = ToSize(t.size);
		Point t_pos = ToPoint(t.position);
		if (r.Width()  != t_size.cx ||
			r.Height() != t_size.cy ||
			r.left     != t_pos.x ||
			r.top      != t_pos.y) {
			r.left = t_pos.x;
			r.top = t_pos.y;
			r.right = r.left + t_size.cx;
			r.bottom = r.top + t_size.cy;
			cw.SetFrameRect0(r);
		}
		
		if (cw.IsPendingLayout()) {
			cw.DeepLayout();
			cw.SetPendingEffectRedraw();
		}
		
		do_render = cw.Redraw(true) || do_render;
	}
	
	Vector<Tuple<Entity*, Transform*, Renderable*>> rends = data->ents->GetComponentsWithEntity<Transform, Renderable>();
	if (rends.GetCount())
		do_render = true;
	
	if (do_render) {
		RenderFrame();
	}
	
	//Panic("");
}

void SDL2GUI3DAlt::RenderFrame() {
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	RefreshGL();
	
	if (data->cam.IsEmpty()) {
		Vector<Tuple<Entity*, Camerable*>> wins = data->ents->GetComponentsWithEntity<Camerable>();
		
		for(int i = 0; i < wins.GetCount(); i++) {
			Tuple<Entity*, Camerable*>& tuple = wins[i];
			ASSERT(tuple.a->HasWeak());
			data->cam = tuple.a->GetSharedFromThis();
			break;
		}
	}
	
	if (data->cam.IsEmpty()) return;
	
	glEnable(GL_DEPTH_TEST);
	RenderCamera();
	
	RenderWindows();
	
	SwapBuffer();


}


void SDL2GUI3DAlt::RenderCamera() {
	
	static TimeStop timer;
	double deltaTime = timer.Seconds();
	
	/*glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_MULTISAMPLE);*/
	
	Vector<Tuple<Entity*, Transform*, Renderable*>> rends = data->ents->GetComponentsWithEntity<Transform, Renderable>();
	
	simple_shader.Use();
    simple_shader.SetInt("texture1", 0);
    simple_shader.SetInt("texture2", 1);
    
    mat4 projection = perspective(GetRadians(100.0f), (float)screen_sz.cx / (float)screen_sz.cy, 0.1f, 100.0f);
    simple_shader.SetMat4("projection", projection);
    
    // camera/view transformation
	Transform& t = *data->cam->Get<Transform>();
	Camerable& c = *data->cam->Get<Camerable>();
	//mat4 view = LookAt(t.position, c.target, vec3(0.0f, 1.0f, 0.0f));
	mat4 view = LookAt((vec3)t.position, (vec3)c.target, vec3(0.0f, 1.0f, 0.0f));
    simple_shader.SetMat4("view", view);
    
    
    // render the loaded model
    /*mat4 model = mat4(1.0f);
    model = translate(model, vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model = scale(model, vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
    float a = deltaTime * 0.1 * 360 / (2 * M_PI);
    model = rotate(model, a, vec3(0.25, 1.0, 0));
    simple_shader.SetMat4("model", model);*/
    //RenderFrame();
	
    
	for(int i = 0; i < rends.GetCount(); i++) {
		Tuple<Entity*, Transform*, Renderable*>& tuple = rends[i];
		Transform& t = *tuple.b.a;
		Renderable& r = *tuple.b.b.a;
		
		if (r.cb) {
			r.cb.Execute(simple_shader);
		}
		else {
			mat4 mat = t.GetMatrix();
			simple_shader.SetMat4("model", mat);
			simple_shader.SetStackMat4(mat);
			
			if (r.model.IsEmpty()) {
				if (r.model_name.GetCount()) {
					LOG("Load model " << r.model_name);
					r.model.Create();
					if (r.model_name == "UnitCube") {
						r.model->LoadModel(FindLocalFile("models" DIR_SEPS "cube.obj"));
					}
					else if (r.model_name == "Gun") {
						r.model->LoadModel(FindLocalFile("models" DIR_SEPS "Gun.glb"));
					}
					else if (r.model_name == "Baseball") {
						r.model->LoadModel(FindLocalFile("models" DIR_SEPS "Baseball.glb"));
					}
					else if (r.model_name == "PaintBrush") {
						r.model->LoadModel(FindLocalFile("models" DIR_SEPS "PaintBrush.glb"));
					}
					else {
						LOG("Unhandled model name " + r.model_name);
						r.model->LoadModel(FindLocalFile("models" DIR_SEPS "cube.obj"));
					}
				}
				else {
					r.model.Create();
					r.model->LoadModel(FindLocalFile("models" DIR_SEPS "cube.obj"));
				}
			}
			
			simple_shader.Paint(*r.GetModel());
		}
		
	}
		
		
	//glDisable(GL_TEXTURE_2D);
}

void SDL2GUI3DAlt::RenderWindows() {
	
	glDisable(GL_DEPTH_TEST);
	
	Transform& t = *data->cam->Get<Transform>();
	Camerable& c = *data->cam->Get<Camerable>();
	
	int width = screen_sz.cx;
	int height = screen_sz.cy;
	mat4 projection = ortho(-width, width, -height, height, -1024.0f, 1024.0f);
    simple_shader.SetMat4("projection", projection);
    
    mat4 view = LookAt(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f, 1.0f, 0.0f));
	simple_shader.SetMat4("view", view);
    
    mat4 ident = identity<mat4>();
    simple_shader.SetMat4("model", ident);
	
	Vector<Tuple<Entity*, Transform2D*, CoreWindow*>> wins = data->ents->GetComponentsWithEntity<Transform2D, CoreWindow>();
	
	for(int i = 0; i < wins.GetCount(); i++) {
		Tuple<Entity*, Transform2D*, CoreWindow*>& tuple = wins[i];
		Transform2D& t = *tuple.b.a;
		CoreWindow& cw = *tuple.b.b.a;
		
		Rect r = cw.GetFrameRect();
		Size t_size = ToSize(t.size);
		Point t_pos = ToPoint(t.position);
		const Framebuffer& fb = cw.GetFramebuffer();
		Size sz = fb.GetSize();
		
		int x0 = width - (t_pos.x) * 2 - (t_size.cx) * 2;
		int y0 = height - (t_pos.y) * 2 - (t_size.cy) * 2;
		int x1 = x0 + t_size.cx * 2;
		int y1 = y0 + t_size.cy * 2;
		
		Model model;
		Mesh& mesh = model.meshes.Add();
		mesh.vertices.Reserve(4);
		mesh.vertices.Add().Set(x0, y0, 0, 0, 1);
		mesh.vertices.Add().Set(x0, y1, 0, 0, 0);
		mesh.vertices.Add().Set(x1, y1, 0, 1, 0);
		mesh.vertices.Add().Set(x1, y0, 0, 1, 1);
		Texture& tex = model.textures.Add();
		tex.width = sz.cx;
		tex.height = sz.cy;
		tex.tex_id = fb.GetTexture();
		mesh.tex_id[TEXTYPE_DIFFUSE] = 0;
		//mesh.textures.Add().img = GetTextureImage(0);
		mesh.indices.Reserve(6);
		mesh.indices.Add(1);
		mesh.indices.Add(2);
		mesh.indices.Add(0);
		mesh.indices.Add(0);
		mesh.indices.Add(2);
		mesh.indices.Add(3);
		mesh.SetupMesh();
		simple_shader.Paint(model);
	}
	
	
}

void SDL2GUI3DAlt::SwapBuffer() {
	SDL_GL_SwapWindow(win);
}

void SDL2GUI3DAlt::RefreshGL() {
	Windows* windows = Ctrl::GetWindows();
	
	// Enable smooth shading
	glShadeModel( GL_SMOOTH );
	
	// Set the background black
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
	// Depth buffer setup
	glClearDepth( 1.0f );
	
	// Enables Depth Testing
	glEnable( GL_DEPTH_TEST );
	
	// The Type Of Depth Test To Do
	glDepthFunc( GL_LEQUAL );
	
	// Really Nice Perspective Calculations
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	
	
	// Culling.
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	
	
	glViewport(0, 0, screen_sz.cx, screen_sz.cy);
	
}












void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color) {
	TODO
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Color color) {
	TODO
}

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
	                        Color ink, int n, const int *dx) {
	TODO
}

void SystemDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
                                    const int *counts, int count_count,
                                    int width, Color color, Color doxor) {
	TODO
}

END_UPP_NAMESPACE
