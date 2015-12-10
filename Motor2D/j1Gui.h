#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"

#define CURSOR_WIDTH 2

enum MouseEvents{
	MouseEnter,
	MouseOut,
	MouseRightClick,
	MouseLeftClick,
};

enum ElementType{
	labels,
	images,
	buttons,
	cursor,
	windows
};

class UI_Element{

public:
	UI_Element(){};
public:

	ElementType type;
	SDL_Rect* box;
	SDL_Texture*  texture;
	p2Point<int> position;
	bool listener = false;
	bool focused = false;

	virtual void Draw();
	virtual void Interact(MouseEvents events);
	virtual void Drag();
	void Debug(bool focus);
};

class Image : public UI_Element{
public:

	Image(SDL_Texture* texture, SDL_Rect* box, ElementType type);
	void Draw();
};

class Label: public UI_Element{
	p2SString text;
	uint maxSize;
	j1Fonts* font;
	SDL_Color color;
	_TTF_Font* font2;
	bool isButton;
	int size;
public:

	Label(p2SString label, SDL_Rect* rect, int size, SDL_Color color, ElementType type, bool isButton);
	Label(Label* label);
	void ChangeColor(SDL_Color color);
	void Draw();
	void Drag();
};

class Cursor : public UI_Element{
public:
	Cursor(SDL_Texture* texture);
	void setPosition(p2Point<int> coords);
	void updatePosition();
	void Draw();
};

class Button : public UI_Element{
	Label* name;
	bool isWindow;
public:
	Button(SDL_Texture* texture, SDL_Rect* rect, Label* label, ElementType type, bool listener, bool isWindow);
	void Draw();
	void Drag();
	void Interact(MouseEvents events);
	Label* GetLabel(){return name;}
};

class Window : public UI_Element{
	p2List<Button*>  buttons;
	p2List<Label*>   labels;
	//InputBoxes
public:
	Window(SDL_Texture* texture, SDL_Rect* rect, Label* label, Button* button, ElementType type, bool listener);
	void Draw();
	void Drag();
	void Interact(MouseEvents events);
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	Image* CreateImage(SDL_Texture* tex, SDL_Rect* rect);
	Label* CreateLabel(p2SString text, SDL_Rect* rect, int size, SDL_Color color, bool isButton);
	Button* CreateButton(SDL_Texture* tex, SDL_Rect* rect,Label* name, bool listener, bool isWindow);
	Window* CreateWindows(SDL_Texture* tex, SDL_Rect* rect, Label* label, Button* button, bool listener);

	bool checkMousePosition(p2Point<int> mousePosition, UI_Element* tmp);

	bool guiEvents(UI_Element* trigger, MouseEvents events);
	
	const SDL_Texture* GetBackGround() const;

private:

	SDL_Texture* background;
	p2SString background_file_name;

	bool debug = false;

public:

	p2List<UI_Element*> elements;

	//Cursor
	Cursor* mouse;
	SDL_Texture* mouseTex;
};

#endif // __j1GUI_H__