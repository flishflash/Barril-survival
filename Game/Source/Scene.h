#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Corazon.h"
#include "Enemy.h"
#include "FlyEnemy.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	iPoint player_initPos;
	FlyEnemy* fly_enemy;
	Enemy* enemy;
	Item* moneda[50];
	Corazon* corazon[3];
	SDL_Texture* originTex = nullptr;
	GuiButton* button;
	int vidas;

	uint timer = 500;
	int scoreFont = -1;
	char scoreText[10] = { "\0" };

	uint highscore = 000;
	char HighscoreText[10] = { "\0" };

	uint coins = 000;
	char coinsc[10] = { "\0" };

	GuiButton* back;

	GuiCheckBox* musv;
	GuiCheckBox* fxv;
	GuiCheckBox* full;
	GuiCheckBox* vsy;

	bool pause = false;

private:
	SDL_Texture* img;
	SDL_Texture* imgr;
	SDL_Texture* imgs;
	SDL_Texture* mouseTileTex = nullptr;

	bool originSelected;
	iPoint origin;
};

#endif // __SCENE_H__