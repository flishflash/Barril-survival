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
	jump = false;
	jump_count = 0;
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x + width / 4, position.y + height / 4, width / 8, bodyType::DYNAMIC);
	return true;
}

bool Player::Update()
{
	b2Vec2 velocity;
	if (jump == false) velocity = b2Vec2(0, -GRAVITY_Y);
	else  velocity = b2Vec2(0, GRAVITY_Y);

		
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jump==false)
		{
				jump = true; 
				jump_count = position.y;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			velocity.x = -5;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			velocity.x = 5;
		}

		if (position.y <= jump_count - 100 && jump==true)
		{
			jump = false;
		}

		pbody->body->SetLinearVelocity(velocity);
	
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - width/4;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - height/4;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();

	return true;
}

bool Player::SaveState(pugi::xml_node& data)
{
	pugi::xml_node pla = data.append_child("player");

	pla.append_attribute("x") = position.x;
	pla.append_attribute("y") = position.y;

	return true;
}
bool Player::CleanUp()
{

	return true;
}