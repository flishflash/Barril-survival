#ifndef __FLYENEMY_H__
#define __FLYENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class FlyEnemy : public Entity
{
public:

	FlyEnemy();
	virtual ~FlyEnemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:
	bool chasing = false;

private:

	SDL_Texture* texture;
	const char* texturePath;

	//DONE 4: Add a physics to an item
	PhysBody* pbody;
	PhysBody* view;
	PhysBody* view2;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation flyAnim;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;
};

#endif // __FLYENEMY_H__