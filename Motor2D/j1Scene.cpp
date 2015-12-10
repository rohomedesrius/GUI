#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	/*
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	
	//debug_tex = App->tex->Load("maps/path2.png");
	*/

	// TODO 3: Create the image (rect {485, 829, 328, 103}) and the text "Hello World" as UI elements

	debug = false;
	dbg = 0;
	v = 0;

	//BackGroundWow
	backGround = App->tex->Load("gui/WoWFons.jpg");
	BG = { 0, 0, 1024, 600 };
	App->gui->CreateImage(backGround, &BG);

	//LogoWow
	logo = App->tex->Load("gui/WoWLogo.png");
	LG = { 0, 0, 428, 200 };
	App->gui->CreateImage(logo, &LG);

	//Labels
	LB1 = { 163, 258, 90, 15 };
	windowLabel = App->gui->CreateLabel("Account Name", &LB1, 16, YELLOW, true);

	LB2 = { 163, 450, 40, 15 };
	button1Label = App->gui->CreateLabel("Login", &LB2, 16, YELLOW, true);

	//Buttons
	BT1 = { 145, 450, 128, 20 };
	button1 = App->tex->Load("gui/ButtonUP.png");
	button1light = App->tex->Load("gui/ButtonLight.png");
	button1down = App->tex->Load("gui/ButtonDown.png");
	windowButton = App->gui->CreateButton(button1, &BT1, button1Label, true, true);

	//Windows
	WD1 = {110, 249, 195, 270};
	windowTex = App->tex->Load("gui/WindowWoW.png");
	App->gui->CreateWindows(windowTex, &WD1, windowLabel, windowButton, true);
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	/*
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}
	*/
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt){

	SDL_ShowCursor(SDL_DISABLE);

	// Gui ---

	if (v == 0){
		focuser = App->gui->elements.start;
		v = 1;
	}

	//Print Elements
	for (p2List_item<UI_Element*>* tmp = App->gui->elements.start; tmp; tmp = tmp->next){
		tmp->data->Draw();
	}

	//Control elements 
	p2Point<int> mousePosition;
	App->input->GetMousePosition(mousePosition.x, mousePosition.y);

	for (p2List_item<UI_Element*>* tmp = App->gui->elements.start; tmp; tmp = tmp->next){
		if ((App->gui->checkMousePosition(mousePosition, tmp->data)) && (tmp->data->listener == true)){
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)){
				App->gui->guiEvents(tmp->data, MouseLeftClick);
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)){
				App->gui->guiEvents(tmp->data, MouseRightClick);
			}
			else{
				App->gui->guiEvents(tmp->data, MouseEnter);
			}
		}
		else{
			App->gui->guiEvents(tmp->data, MouseOut);
		}
	}

	// ---------------------
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	//DEBUG--------------------------------
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN){
		if (dbg == 0){
			debug = true;
			dbg = 1;
		}
		else{
			debug = false;
			dbg = 0;
		}
	}
	if (debug){
		p2List_item<UI_Element*>* tmp;
		focuser->data->focused = true;
		for (tmp = App->gui->elements.start; tmp; tmp = tmp->next){
			if (tmp->data->focused){
				tmp->data->Debug(true);
			}
			else{
				tmp->data->Debug(false);
			}
		}
		//TABULADOR
		if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN){
			focuser = focuser->next;
			focuser->prev->data->focused = false;
			if (focuser->next == NULL)
				focuser = App->gui->elements.start;
		}
	}
	//-------------------------------------

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	/*
	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}
	*/

	App->gui->mouse->updatePosition();
	App->gui->mouse->Draw();
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	
	App->tex->UnLoad(debug_tex);
	App->tex->UnLoad(backGround);
	App->tex->UnLoad(logo);
	App->tex->UnLoad(button1);
	App->tex->UnLoad(button1light);
	App->tex->UnLoad(button1down);
	App->tex->UnLoad(windowTex);
	delete windowButton;
	delete button1Label;
	delete windowLabel;

	return true;
}
