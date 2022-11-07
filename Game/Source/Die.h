#ifndef __DIE_H__
#define __DIE_H__

#include "Module.h"

struct SDL_Texture;

class Die : public Module
{
public:

	Die();

	// Destructor
	virtual ~Die();

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

#endif // __DIE_H__