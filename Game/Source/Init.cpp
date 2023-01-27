#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Window.h"
#include "Init.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

Init::Init() : Module()
{
	name.Create("Init");
}

// Destructor
Init::~Init()
{}

// Called before render is available
bool Init::Awake()
{
	LOG("Loading Init");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool Init::Start()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	img = app->tex->Load("Assets/Maps/Titulo_BS.png");
	app->audio->PlayMusic("Assets/Audio/Music/Game_Over.ogg");

	uint w, h;
	app->win->GetWindowSize(w, h);
	newGame = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 100,(int)h / 2 + 100,200,40 }, this);
	Continue = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 100,(int)h / 2 + 150,200,40 }, this);
	credits = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 75,(int)h / 2 + 210,150,30 }, this);
	exit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 75,(int)h / 2 + 250,150,30 }, this);
	settings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w - 100,50,50,50 }, this);
	return true;
}

// Called each loop iteration
bool Init::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Init::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 5;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 5;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 5;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 5;

	// Placeholder not needed any more
	app->render->DrawTexture(img, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || newGame->state == GuiControlState::PRESSED) {
		app->fade->FadeToblack(this, (Module*)app->scene, 20);
		app->scene->Start();
	}

	if (Continue->state == GuiControlState::PRESSED)
	{
		app->fade->FadeToblack(this, (Module*)app->scene, 20);
		app->scene->Start();
		app->LoadGameRequest();
	}



	app->guiManager->Draw();
	return true;
}

// Called each loop iteration
bool Init::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || exit->state == GuiControlState::PRESSED)
		ret = false;

	return ret;
}

// Called before quitting
bool Init::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
