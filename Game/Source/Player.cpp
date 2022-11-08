#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "FadeToBlack.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/Jump.ogg");
	dieFx = app->audio->LoadFx("Assets/Audio/Fx/Death_Sound.ogg");

	return true;
}

bool Player::Update()
{

	b2Vec2 velocity;
	if (up == true) velocity = b2Vec2(0, GRAVITY_Y);
	else  velocity = b2Vec2(0, -GRAVITY_Y);

		if (position.x + app->render->camera.x > 800)
		{
			app->render->camera.x -= 4;
		}
		if (position.x + app->render->camera.x < 200)
		{
			app->render->camera.x += 4;
		}
		if (position.y + app->render->camera.y > 625)
		{
			app->render->camera.y -= 4;
		}
		if (position.y + app->render->camera.y < 400)
		{
			app->render->camera.y += 4;
		}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jump == false)
	{
		jump = true;
		up = true;
		jump_count = position.y;
		app->audio->PlayFx(jumpFx);

	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		velocity.x = -5;
		flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		velocity.x = 5;
		flip = SDL_RendererFlip::SDL_FLIP_NONE;
	}

	if (position.y <= (jump_count - 150) && jump==true)
	{
		up = false;
	}

	pbody->body->SetLinearVelocity(velocity);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 48;

	app->render->DrawTexture(texture, position.x , position.y, NULL, 1.0f, NULL, NULL, NULL, flip);

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			if (up == true) up == false; jump_count += position.y + 2000;
			if (up==false) jump=false;
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		case ColliderType::WATER:
			LOG("Collision WATER");
			app->audio->PlayFx(dieFx);
			app->physics->active = false;
			app->fade->FadeToblack((Module*)app->scene, (Module*)app->die, 50);
			app->render->camera.x = 0;
			app->render->camera.y = 0;


			break;
	}
	
}
