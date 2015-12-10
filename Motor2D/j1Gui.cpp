#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Scene.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	background_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	background = App->tex->Load(background_file_name.GetString());

	mouseTex = App->tex->Load("gui/Cursor.png");
	mouse = new Cursor(mouseTex);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	delete mouse;
	App->tex->UnLoad(mouseTex);

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetBackGround() const
{
	return background;
}

// CLASS GUI---------------------------------------------------

void UI_Element::Draw(){}
void UI_Element::Interact(MouseEvents events){}

void UI_Element::Debug(bool focus){
	if (!focus)
		App->render->DrawQuad(*box, 255, 255, 0, 255, false);
	else
		App->render->DrawQuad(*box, 0, 0, 255, 255, false);
}

void UI_Element::Drag(){
	p2Point<int> mouseMotion;
	App->input->GetMouseMotion(mouseMotion.x, mouseMotion.y);
	box->x = box->x + mouseMotion.x;
	box->y = box->y + mouseMotion.y;
}

Image* j1Gui::CreateImage(SDL_Texture* tex, SDL_Rect* rect){
	Image* image = new Image(tex, rect, images);
	elements.add(image);
	return image;
}

Label* j1Gui::CreateLabel(p2SString text, SDL_Rect* rect, int size, SDL_Color color, bool isButton){
	Label* label = new Label(text, rect, size, color, labels, isButton);
	if (!isButton){
		elements.add(label);
	}
	return label;
}

Button* j1Gui::CreateButton(SDL_Texture* tex, SDL_Rect* rect, Label* label, bool listener, bool isWindow){
	Button* button = new Button(tex, rect, label, buttons, listener, isWindow);
	if (!isWindow){
		elements.add(button);
		elements.add(label);
	}
	return button;
}

Window* j1Gui::CreateWindows(SDL_Texture* tex, SDL_Rect* rect, Label* label, Button* button, bool listener){
	Window* window = new Window(tex, rect, label, button, windows, listener);
	elements.add(window);
	elements.add(button);
	elements.add(button->GetLabel());
	elements.add(label);
	return window;
}

bool j1Gui::checkMousePosition(p2Point<int> mousePosition, UI_Element* tmp){

	return ((mousePosition.x > tmp->box->x) && (mousePosition.x < tmp->box->x + tmp->box->w) && (mousePosition.y > tmp->box->y) && (mousePosition.y < tmp->box->y + tmp->box->h));

}

// Events
bool j1Gui::guiEvents(UI_Element* trigger, MouseEvents events){
	switch (events){
	case MouseEnter:
		trigger->Interact(MouseEnter);
		return true;
		break;
	case MouseOut:
		trigger->Interact(MouseOut);
		return true;
		break;
	case MouseRightClick:
		trigger->Drag();
		return true;
		break;
	case MouseLeftClick:
		trigger->Interact(MouseLeftClick);
		return true;
		break;
	default:
		return false;
		break;
	}
}

//-------------------------------------

//Image
Image::Image(SDL_Texture* texture, SDL_Rect* box, ElementType type){
	this->texture = texture;
	this->box = box;
	this->type = type;
}

void Image::Draw(){
	App->render->Blit(texture, box->x, box->y, box, false);
}
//-------------------------------------

//Label
Label::Label(p2SString label, SDL_Rect* rect, int size, SDL_Color color, ElementType type, bool isButton) : maxSize(14){
	text = label;
	font2 = App->font->Load(WOW_FONT, size);
	this->box = rect;
	this->color = color;
	this->type = type;
	this->isButton = isButton;
	this->size = size;
}

Label::Label(Label* label) : maxSize(14){
	text = label->text;
	font2 = App->font->Load(WOW_FONT, label->size);
	this->box = label->box;
	this->color = label->color;
	this->type = label->type;
	this->isButton = label->isButton;
}

void Label::Draw(){
	App->render->Blit(App->font->Print(text.GetString(), color, font2), box->x, box->y, false, false);
}

void Label::ChangeColor(SDL_Color color){
	this->color = color;
}

void Label::Drag(){
	p2Point<int> mouseMotion;
	App->input->GetMouseMotion(mouseMotion.x, mouseMotion.y);
	box->x = box->x + mouseMotion.x;
	box->y = box->y + mouseMotion.y;
}
//-------------------------------------

//Cursor
Cursor::Cursor(SDL_Texture* texture){
	this->texture = texture;
	position.x = 0;
	position.y = 0;
}
void Cursor::setPosition(p2Point<int> coords){
	position.x = coords.x;
	position.y = coords.y;
}
void Cursor::updatePosition(){
	p2Point<int> mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);
	setPosition(mouse);
}
void Cursor::Draw(){
	App->render->Blit(texture, position.x, position.y, false);
}
//---------------------------------------

//Button
Button::Button(SDL_Texture* texture, SDL_Rect* rect, Label* label, ElementType type, bool listener, bool isWindow){
	this->texture = texture;
	name = new Label(label);
	box = rect;
	name->box->x = box->x + (box->w / 2) - (name->box->w / 2);
	name->box->y = box->y + 2;
	this->type = type;
	this->listener = listener;
	position.x = rect->x;
	position.y = rect->y;
	this->isWindow = isWindow;
}
void Button::Draw(){
	App->render->Blit(texture, box->x, box->y, false);
}

void Button::Drag(){
	p2Point<int> mouseMotion;
	App->input->GetMouseMotion(mouseMotion.x, mouseMotion.y);
	box->x = box->x + mouseMotion.x;
	box->y = box->y + mouseMotion.y;
	name->box->x = box->x + (box->w / 2) - (name->box->w / 2);
	name->box->y = box->y + 2;
}

void Button::Interact(MouseEvents events){
	if (events == MouseEnter){
		texture = App->scene->button1light;
	}
	if (events == MouseLeftClick){
		texture = App->scene->button1down;
	}
	if (events == MouseOut){
		texture = App->scene->button1;
	}
}
//-------------------------------------------------

//Window
Window::Window(SDL_Texture* texture, SDL_Rect* rect, Label* label, Button* button, ElementType type, bool listener){
	this->texture = texture;
	this->box = rect;
	this->labels.add(label);
	this->buttons.add(button);
	this->type = type;
	this->listener = listener;
}

void Window::Draw(){
	App->render->Blit(texture, box->x, box->y, false);
}

void Window::Drag(){
	p2Point<int> mouseMotion;
	App->input->GetMouseMotion(mouseMotion.x, mouseMotion.y);
	box->x = box->x + mouseMotion.x;
	box->y = box->y + mouseMotion.y;
	for (p2List_item<Button*>* tmp = this->buttons.start; tmp; tmp = tmp->next){
		tmp->data->Drag();
	}
	for (p2List_item<Label*>* tmp = this->labels.start; tmp; tmp = tmp->next){
		tmp->data->Drag();
	}
}

void Window::Interact(MouseEvents events){}
//-------------------------------------------------

