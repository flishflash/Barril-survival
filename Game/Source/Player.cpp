#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Die.h"
#include "Win.h"
#include "Point.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "EntityManager.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//animation load
	idleAnim.PushBack({ 3, 15, 30, 68 });
	idleAnim.PushBack({ 52, 16, 29, 67 });
	idleAnim.PushBack({ 100, 16, 31, 69 });
	idleAnim.loop = true;
	idleAnim.speed = 0.01f;
	currentAnimation = &idleAnim;
	//Run
	runAnim.PushBack({ 6, 115, 36, 68 });
	runAnim.PushBack({ 61, 118, 34, 66 });
	runAnim.PushBack({ 112, 117, 35, 67 });
	runAnim.PushBack({ 169, 118, 36, 65 });
	runAnim.PushBack({ 228, 118, 34, 65 });
	runAnim.PushBack({ 284, 118, 36, 65 });
	runAnim.loop = true;
	runAnim.speed = 0.15f;
	//Jump
	jumpAnim.PushBack({ 10, 233, 30, 73 });
	jumpAnim.PushBack({ 56, 233, 27, 67 });
	jumpAnim.PushBack({ 100, 232, 27, 73 });
	jumpAnim.loop = true;
	jumpAnim.speed = 0.1f;
	//Die
	dieAnim.PushBack({ 8, 354, 39, 68 });
	dieAnim.PushBack({ 57, 351, 54, 67 });
	dieAnim.PushBack({ 112, 355, 76, 72 });
	dieAnim.loop = false;
	dieAnim.speed = 0.02f;

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	//Load cositas
	app->die->active = false;
	app->winw->active = false;
	this->position.x = 1312;
	this->position.y = 2000;

	pbody = app->physics->CreateRectangle(position.x, position.y, 32, 60, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;
	
	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/Jump.ogg");
	dieFx = app->audio->LoadFx("Assets/Audio/Fx/Death_Sound.ogg");
	pbody->body->SetFixedRotation(true);

	die = false;

	return true;
}

bool Player::Update()
{
	if (!app->physics->debug)
	{
		if (die == false)
		{
			if (jump != true && die != true)currentAnimation = &idleAnim;

			b2Vec2 velocity;
			if (up == true) velocity = b2Vec2(0, GRAVITY_Y);
			else  velocity = b2Vec2(0, -GRAVITY_Y);


			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jump == false)
			{
				jump = true;
				up = true;
				jump_count = position.y;
				app->audio->PlayFx(jumpFx);
				currentAnimation = &jumpAnim;

			}

			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				velocity.x = -5;
				flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
				if (jump != true && die != true)currentAnimation = &runAnim;
				if (position.x + app->render->camera.x < 200)
				{
					app->render->camera.x += 5;
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				velocity.x = 5;
				flip = SDL_RendererFlip::SDL_FLIP_NONE;
				if (jump != true && die != true)currentAnimation = &runAnim;
				if (position.x + app->render->camera.x > 800)
				{
					app->render->camera.x -= 5;
				}
			}

			if (position.y <= (jump_count - 120) && jump == true)
			{
				up = false;
			}

			pbody->body->SetLinearVelocity(velocity);

			//Update player position in pixels
			position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
			position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 36;

			if (position.y + app->render->camera.y > 625)
			{
				app->render->camera.y -= 5;
			}
			if (position.y + app->render->camera.y < 400)
			{
				app->render->camera.y += 5;
			}

		}

		currentAnimation->Update();

		app->render->DrawTexture(texture, position.x, position.y, &(currentAnimation->GetCurrentFrame()), 1.0f, NULL, NULL, NULL, flip);

		if (die == true && currentAnimation->HasFinished() == true)
		{
			app->fade->FadeToblack((Module*)app->scene, (Module*)app->die, 50);
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			CleanUp();
		}
	}
	else
	{
		b2Vec2 velocity(0,0);

		currentAnimation = &idleAnim;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			velocity.x = -5;
			flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
			if (position.x + app->render->camera.x < 200)
			{
				app->render->camera.x += 5;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			velocity.x = 5;
			flip = SDL_RendererFlip::SDL_FLIP_NONE;
			if (position.x + app->render->camera.x > 800)
			{
				app->render->camera.x -= 5;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			velocity.y = -5;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			velocity.y = 5;
		}

		pbody->body->SetLinearVelocity(velocity);

		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 36;

		if (position.y + app->render->camera.y > 625)
		{
			app->render->camera.y -= 5;
		}
		if (position.y + app->render->camera.y < 400)
		{
			app->render->camera.y += 5;
		}

		currentAnimation->Update();

		app->render->DrawTexture(texture, position.x, position.y, &(currentAnimation->GetCurrentFrame()), 1.0f, NULL, NULL, NULL, flip);

	}

	return true;
}

bool Player::CleanUp()
{
	delete pbody;
	pbody = NULL;
	app->physics->active = false;
	this->active = false;
	dieAnim.FullReset();

	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			currentAnimation = &dieAnim;
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
			die = true;
			currentAnimation = &dieAnim;
			app->audio->PlayFx(dieFx);

			break;
		case ColliderType::WIN:
			LOG("Collision WIN");
			app->fade->FadeToblack((Module*)app->scene, (Module*)app->winw, 50);
			app->render->camera.x = 0;
			app->render->camera.y = 0;

			app->audio->PlayMusic("Assets/Audio/Music/Victory.ogg");
			break;
	}
	
}
