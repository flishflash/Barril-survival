#ifndef __INIT_H__
#define __INIT_H__

#include "Module.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class Init : public Module
{
public:

	Init();

	// Destructor
	virtual ~Init();

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

public:
	GuiButton* newGame;
	GuiButton* Continue;
	GuiButton* credits;
	GuiButton* settings;
	GuiButton* exit;
	GuiButton* back;

	GuiCheckBox* musv;
	GuiCheckBox* fxv;
	GuiCheckBox* full;
	GuiCheckBox* vsy;
private:
	SDL_Texture* img;
	SDL_Texture* img2;
	SDL_Texture* img3;
	SDL_Texture* img4;

	pugi::xml_node fullscreen;
	pugi::xml_node music;
	pugi::xml_node fx;
	pugi::xml_node vsync;

	pugi::xml_document config;
	pugi::xml_parse_result configg;
	Uint32 flags = SDL_WINDOW_SHOWN;
};

#endif // __INIT_H__