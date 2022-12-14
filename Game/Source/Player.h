#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player(App* app, bool start_enabled = false);
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	SDL_RendererFlip flip;

	PhysBody* pbody;

	bool die;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	bool jump = false;
	bool up = false;
	int jump_count = 0;

	// L07 DONE 5: Add physics to the player - declare a Physics body

	int pickCoinFxId;
	int jumpFx;
	int dieFx;

	//Animation 
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnim;
	Animation jumpAnim;
	Animation runAnim;
	Animation dieAnim;

};

#endif // __PLAYER_H__