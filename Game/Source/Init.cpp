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
	img2 = app->tex->Load("Assets/Maps/Titulos_B.png");
	img3 = app->tex->Load("Assets/Maps/settings.png");
	img4 = app->tex->Load("Assets/Maps/Credits.png");
	app->audio->PlayMusic("Assets/Audio/Music/Game_Over.ogg");

	uint w, h;
	app->win->GetWindowSize(w, h);
	newGame = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 110,(int)h / 2 + 90,210,40 }, this);
	Continue = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 110,(int)h / 2 + 140,210,40 }, this);
	credits = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 80,(int)h / 2 + 200,150,30 }, this);
	exit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 45,(int)h / 2 + 240,75,30 }, this);
	settings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w - 102,40,60,60 }, this);
	back = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w - 102,600,60,60 }, this);
	musv = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "Button 1", { (int)w /2,210,60,60 }, this);
	fxv = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "Button 1", { (int)w /2,300,60,60 }, this);
	full = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "Button 1", { (int)w /2,425,60,60 }, this);
	vsy = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "Button 1", { (int)w /2,520,60,60 }, this);
	settings->state = GuiControlState::NORMAL;
	Continue->state = GuiControlState::NORMAL;
	newGame->state = GuiControlState::NORMAL;
	credits->state = GuiControlState::NORMAL;
	exit->state = GuiControlState::NORMAL;
	back->state = GuiControlState::DISABLED;
	musv->state = GuiControlState::DISABLED;
	fxv->state = GuiControlState::DISABLED;
	full->state = GuiControlState::DISABLED;
	vsy->state = GuiControlState::DISABLED;

	configg = config.load_file("config.xml");
	app->secondsSinceStartup = 0;
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
		settings->state = GuiControlState::DISABLED;
		Continue->state = GuiControlState::DISABLED;
		newGame->state = GuiControlState::DISABLED;
		credits->state = GuiControlState::DISABLED;
		exit->state = GuiControlState::DISABLED;
		back->state = GuiControlState::DISABLED;
	}

	if (Continue->state == GuiControlState::PRESSED)
	{
		app->fade->FadeToblack(this, (Module*)app->scene, 20);
		app->LoadGameRequest();
		settings->state = GuiControlState::DISABLED;
		Continue->state = GuiControlState::DISABLED;
		newGame->state = GuiControlState::DISABLED;
		credits->state = GuiControlState::DISABLED;
		exit->state = GuiControlState::DISABLED;
		back->state = GuiControlState::DISABLED;
		app->scene->Start();
		app->render->camera.x = app->scene->player->position.x;
		app->render->camera.y = app->scene->player->position.y;
		
	}
	if (settings->state == GuiControlState::PRESSED)
	{
		app->render->camera.y = 725;
		settings->state = GuiControlState::DISABLED;
		Continue->state = GuiControlState::DISABLED;
		newGame->state = GuiControlState::DISABLED;
		credits->state = GuiControlState::DISABLED;
		exit->state = GuiControlState::DISABLED;
		back->state = GuiControlState::NORMAL;

		musv->state = GuiControlState::SELECTED;
		fxv->state = GuiControlState::SELECTED;
		full->state = GuiControlState::NORMAL;
		vsy->state = GuiControlState::NORMAL;
	}
	if (credits->state == GuiControlState::PRESSED)
	{
		app->render->camera.y = -725;
		settings->state = GuiControlState::DISABLED;
		Continue->state = GuiControlState::DISABLED;
		newGame->state = GuiControlState::DISABLED;
		credits->state = GuiControlState::DISABLED;
		exit->state = GuiControlState::DISABLED;
		back->state = GuiControlState::NORMAL;
	}
	if (back->state == GuiControlState::PRESSED)
	{
		app->render->camera.y = 0;
		settings->state = GuiControlState::NORMAL;
		Continue->state = GuiControlState::NORMAL;
		newGame->state = GuiControlState::NORMAL;
		credits->state = GuiControlState::NORMAL;
		exit->state = GuiControlState::NORMAL;
		back->state = GuiControlState::DISABLED;

		musv->state = GuiControlState::DISABLED;
		fxv->state = GuiControlState::DISABLED;
		full->state = GuiControlState::DISABLED;
		vsy->state = GuiControlState::DISABLED;
	}

	if (musv->state == GuiControlState::SELECTED)
	{
		config.child("audio").child("music").attribute("volume") = 0;
	}
	if (musv->state == GuiControlState::NORMAL)
	{
		config.child("audio").child("music").attribute("volume") = 128;
	}

	if (fxv->state == GuiControlState::SELECTED)
	{
		config.child("audio").child("fx").attribute("volume") = 0;
	}
	if (fxv->state == GuiControlState::NORMAL)
	{
		config.child("audio").child("fx").attribute("volume") = 128;
	}

	if (vsy->state == GuiControlState::SELECTED)
	{
		config.child("audio").child("fx").attribute("value") = true;
	}
	if (vsy->state == GuiControlState::NORMAL)
	{
		config.child("audio").child("fx").attribute("value") = false;
	}

	if (full->state == GuiControlState::SELECTED)
	{
	
	}
	if (full->state == GuiControlState::NORMAL)
	{

	}


	app->render->DrawTexture(img3, 0, -725);
	app->render->DrawTexture(img4, 0, 725);
	app->guiManager->Draw();
	app->render->DrawTexture(img2, 0, 0);

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
