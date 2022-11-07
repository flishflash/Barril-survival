#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Die.h"

#include "Defs.h"
#include "Log.h"

Die::Die() : Module()
{
	name.Create("scene");
}

// Destructor
Die::~Die()
{}

// Called before render is available
bool Die::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Die::Start()
{
	img = app->tex->Load("Assets/Maps/Game_Over_Barril_Survival.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Die::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Die::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	// Placeholder not needed any more
	app->render->DrawTexture(img, 0, 0);
	if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) app->fade->FadeToblack(this, (Module*)app->scene, 50);

	return true;
}

// Called each loop iteration
bool Die::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Die::CleanUp()
{
	LOG("Freeing scene");

	return true;
}