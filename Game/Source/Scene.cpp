#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "Die.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	
	for (pugi::xml_node itemNode = config.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
	{
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = itemNode;
	}	
	for (pugi::xml_node itemNode = config.child("fly_enemy"); itemNode; itemNode = itemNode.next_sibling("fly_enemy"))
	{
		fly_enemy = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLY_ENEMY);
		fly_enemy->parameters = itemNode;
	}

	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	player_initPos.x = config.child("player").attribute("x").as_int();
	player_initPos.y = config.child("player").attribute("y").as_int();

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	LOG("Loading Scene");
	img = app->tex->Load("Assets/Maps/back.png");
	app->audio->PlayMusic("Assets/Audio/Music/Map_Music.ogg");
	app->render->DrawTexture(img, 0, 0);

	app->physics->active = true;
	app->physics->Start();
	player->active = true;
	enemy->active = true;
	fly_enemy->active = true;
	player->Start();

	// L03: DONE: Load map
	app->map->Load();
	app->entityManager->active = true;
	enemy->Start();
	fly_enemy->Start();

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	app->render->camera.x = -800;
	app->render->camera.y = -1455;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 5;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 5;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 5;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 5;

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		app->fade->FadeToblack(this, (Module*)app->die, 50); 
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		player->die = true;
		player->CleanUp();
		enemy->CleanUp();
		fly_enemy->CleanUp();
	}

	app->render->DrawTexture(img, 0, 0);

	// Draw map
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	img = NULL;
	app->scene->active = false;
	app->entityManager->active = false;
	player->position = player_initPos;

	return true;
}
