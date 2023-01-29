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
#include "Pathfinding.h"
#include "ModuleFonts.h"

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
	int n=0;
	for (pugi::xml_node itemNode = config.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
	{
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = itemNode;
		enemy->ID = n;
		n++;
	}	
	n = 0;
	for (pugi::xml_node itemNode = config.child("fly_enemy"); itemNode; itemNode = itemNode.next_sibling("fly_enemy"))
	{
		fly_enemy = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLY_ENEMY);
		fly_enemy->parameters = itemNode;
		fly_enemy->ID = n;
		n++;
	}
	n = 0;
	for (pugi::xml_node itemNode = config.child("corazon"); itemNode; itemNode = itemNode.next_sibling("corazon"))
	{
		corazon = (Corazon*)app->entityManager->CreateEntity(EntityType::CORAZON);
		corazon->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		moneda[n] = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		moneda[n]->parameters = itemNode;
		n++;
	}
	n = 0;
	
	LOG(" numro %d", n);
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

	//Active physics
	app->physics->active = true;
	app->physics->Start();

	//Active entities
	app->entityManager->active = true;

	//Active enemies
	enemy->active = true;
	fly_enemy->active = true;
	enemy->Start();
	fly_enemy->Start();
	app->pathfinding->ClearLastPath();

	//Restart player
	player->active = true;
	player->Start();
	player->position = player_initPos;

	for (int n = 0; n < 14; n++)
	{
		moneda[n]->Start();
	}

	// L03: DONE: Load map
	bool retLoad = app->map->Load();

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

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {

		// Texture to highligh mouse position 
		mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

		// Texture to show path origin 
		originTex = app->tex->Load("Assets/Maps/path_square.png");
	}
	vidas = 3;
	char lookupTable[] = { "ABCDEFGHIJKLNOPKRSTUVXYZ0123456789: " };
	scoreFont = app->fonts->Load("Assets/Textures/ABC.png", lookupTable, 1);
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
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) { app->SaveGameRequest(); app->pathfinding->ClearLastPath(); }

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) { app->LoadGameRequest(); app->pathfinding->ClearLastPath(); }

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT) 
	{ 
		int check = 1;
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) { check += 1; }
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) { check -= 1; }
		switch (check)
		{
		case 1:
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(-1312), PIXEL_TO_METERS(-2000) }, 0);
		case 2:
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(-1360), PIXEL_TO_METERS(-1664) }, 0);
		case 3:
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(-1360), PIXEL_TO_METERS(-1152) }, 0);
		case 4:
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(-2032), PIXEL_TO_METERS(-1216) }, 0);
		case 5:
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(-2416), PIXEL_TO_METERS(-1216) }, 0);
		case 6:
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(-2672), PIXEL_TO_METERS(-1024) }, 0);
		}


	}

	float speed = 0.2*dt;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= ceil(speed);

	//Start from lv1
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->render->camera.x = -800;
		app->render->camera.y = -1455;
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(1312), PIXEL_TO_METERS(2000) },0);
	}
	//Start from current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->render->camera.x = -800;
		app->render->camera.y = -1455;
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(1312), PIXEL_TO_METERS(2000) }, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		app->fade->FadeToblack(this, (Module*)app->die, 50); 
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		player->dies = true;
		player->CleanUp();
		enemy->CleanUp();
		fly_enemy->CleanUp();
	}
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		app->fade->FadeToblack(this, (Module*)app->winw, 50);
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		player->dies = true;
		player->CleanUp();
		enemy->CleanUp();
		fly_enemy->CleanUp();
	}

	app->render->DrawTexture(img, 0, 0);

	// Draw map
	app->map->Draw();

	if (app->physics->debug)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		iPoint mouseTile = iPoint(0, 0);

		if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
			mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x - app->map->mapData.tileWidth / 2,
				mouseY - app->render->camera.y - app->map->mapData.tileHeight / 2);
		}
		if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
			mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
				mouseY - app->render->camera.y);
		}

		//Convert again the tile coordinates to world coordinates to render the texture of the tile
		iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
		app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

		//Test compute path function
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (originSelected == true)
			{
				app->pathfinding->CreatePath(origin, mouseTile);
				originSelected = false;
			}
			else
			{
				origin = mouseTile;
				originSelected = true;
				app->pathfinding->ClearLastPath();
			}
		}

		// L12: Get the latest calculated path and draw
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}

		// L12: Debug pathfinding
		iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
		app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
	}
	
	sprintf_s(scoreText, 10, "%7d", timer);
	sprintf_s(HighscoreText, 10, "%7d", player->vidas);
	sprintf_s(coinsc, 10, "%7d", coins);

	if (timer > 0)
	{
		timer = 500 - app->secondsSinceStartup;
	}
	else
	{
		player->dies = true;
	}

	app->fonts->BlitText(900, 35, scoreFont, scoreText);

	app->fonts->BlitText(35, 35, scoreFont, "LIVES  X");
	app->fonts->BlitText(885, 35, scoreFont, "TIME");

	app->fonts->BlitText(50, 35, scoreFont, HighscoreText);
	app->fonts->BlitText(35, 70, scoreFont, "COINS");
	app->fonts->BlitText(50, 70, scoreFont, coinsc);

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
	app->map->CleanUp();
	app->scene->active = false;
	app->entityManager->active = false;
	player->position = player_initPos;

	return true;
}
