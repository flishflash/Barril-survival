#include "Win.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

Win::Win(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene");
}

// Destructor
Win::~Win()
{}

// Called before render is available
bool Win::Awake()
{
	LOG("Loading Scene");
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
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		app->fade->FadeToblack(this, (Module*)app->scene, 50);
		app->audio->PlayMusic("Assets/Audio/Music/Map_Music.ogg");
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