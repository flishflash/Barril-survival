#ifndef __WIN_H__
#define __WIN_H__

#include "Module.h"

struct SDL_Texture;
class Win : public Module
{
public:

	Win(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Win();

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

private:
	SDL_Texture* img;

};
#endif // __WIN_H__