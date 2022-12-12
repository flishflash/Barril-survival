#include "Win.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Audio.h"
#include "Scene.h"
#include "Render.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

Win::Win() : Module()
{
	name.Create("Win");
}

// Destructor
Win::~Win()
{}

// Called before render is available
bool Win::Awake()
{
	LOG("Loading WIN");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool Win::Start()
{
	img = app->tex->Load("Assets/Maps/Victory_BS.png");
	app->audio->PlayMusic("Assets/Audio/Music/Victory.ogg");
	return true;
}

// Called each loop iteration
bool Win::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Win::Update(float dt)
{
	app->render->DrawTexture(img, 0, 0);
	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		app->fade->FadeToblack(this, (Module*)app->scene, 50);
		app->scene->Start();
	}
	

	return true;
}

// Called each loop iteration
bool Win::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Win::CleanUp()
{
	LOG("Freeing scene");

	return true;
}