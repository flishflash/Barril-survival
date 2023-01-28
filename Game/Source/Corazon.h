#ifndef __CORAZON_H__
#define __CORAZON_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Corazon : public Entity
{
public:

	Corazon();
	virtual ~Corazon();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;

	//DONE 4: Add a physics to an item
	PhysBody* pbody;
};

#endif // __CORAZON_H__