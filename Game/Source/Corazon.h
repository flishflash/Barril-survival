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
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;
};

#endif // __CORAZON_H__