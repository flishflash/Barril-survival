#include "FlyEnemy.h"
#include "App.h"
#include "Animation.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

FlyEnemy::FlyEnemy() : Entity(EntityType::FLY_ENEMY)
{
	name.Create("FlyEnemy");
}

FlyEnemy::~FlyEnemy() {}

bool FlyEnemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//Idle
	idleAnim.PushBack({ 1, 2, 51, 57 });
	idleAnim.PushBack({ 64, 4, 50, 55 });
	idleAnim.PushBack({ 135, 13, 41, 45 });
	idleAnim.PushBack({ 204, 15, 34, 42 });
	idleAnim.PushBack({ 267, 15, 33, 43 });
	idleAnim.PushBack({ 204, 15, 34, 42 });
	idleAnim.PushBack({ 135, 13, 41, 45 });
	idleAnim.PushBack({ 64, 4, 50, 55 });
	idleAnim.PushBack({ 1, 2, 51, 57 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;
	currentAnimation = &idleAnim;

	//Flying
	flyAnim.PushBack({ 562, 7, 51, 43 });
	flyAnim.PushBack({ 623, 22, 52, 20 });
	flyAnim.PushBack({ 687, 18, 51, 23 });
	flyAnim.PushBack({ 623, 22, 52, 20 });
	flyAnim.PushBack({ 562, 7, 51, 43 });
	flyAnim.loop = false;
	flyAnim.speed = 0.02f;

	return true;
}

bool FlyEnemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool FlyEnemy::Update()
{
	currentAnimation->Update();
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x + 8, position.y + 8, &(currentAnimation->GetCurrentFrame()));

	return true;
}

bool FlyEnemy::CleanUp()
{
	this->active = false;
	return true;
}