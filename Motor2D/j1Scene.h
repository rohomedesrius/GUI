#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class Label;
class Button;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* debug_tex;
	GuiImage* banner;
	GuiText* text;
	//Background
	SDL_Texture* backGround;
	SDL_Rect BG;
	//Logo
	SDL_Texture* logo;
	SDL_Rect LG;
	//Labels
	SDL_Rect LB1;
	SDL_Rect LB2;
public:
	SDL_Texture* button1;
	SDL_Texture* button1light;
	SDL_Texture* button1down;
private:
	//Buttons
	Button* windowButton;
	SDL_Rect BT1;
	Label* button1Label;
	
	//Windows
	SDL_Rect WD1;
	SDL_Texture* windowTex;

	bool debug;
	int dbg,v;
	p2List_item<UI_Element*>* focuser;
};

#endif // __j1SCENE_H__