#ifndef __INIT_H__
#define __INIT_H__

#include "Module.h"
#include "GuiButton.h"

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
private:
	SDL_Texture* img;
	SDL_Texture* img2;
	SDL_Texture* img3;
	SDL_Texture* img4;

};

#endif // __INIT_H__