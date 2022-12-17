#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();
	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	bool chasing = false;
	bool originSelected;

private:

	SDL_Texture* texture;
	const char* texturePath;
	SDL_Texture* mouseTileTex;
	SDL_Texture* originTex;

	Animation* currentAnimation = nullptr;
	Animation walkAnim;
	Animation dieAnim;

	PhysBody* view;

	//DONE 4: Add a physics to an item
	PhysBody* pbody;
	PhysBody* view2;

	iPoint origin;
};

#endif // __ENEMY_H__