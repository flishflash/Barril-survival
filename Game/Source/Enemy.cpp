#include "Enemy.h"
#include "Animation.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");
}

Enemy::~Enemy() {}

bool Enemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//Walk
	walkAnim.PushBack({ 2, 10, 28, 14 });
	walkAnim.PushBack({ 34, 10, 28, 14 });
	walkAnim.PushBack({ 66, 10, 28, 14 });
	walkAnim.PushBack({ 98, 11, 28, 13 });
	walkAnim.PushBack({ 132, 12, 26, 12 });
	walkAnim.PushBack({ 164, 12, 26, 12 });
	walkAnim.PushBack({ 196, 12, 24, 12 });
	walkAnim.PushBack({ 228, 13, 26, 11 });
	walkAnim.loop = true;
	walkAnim.speed = 0.1f;
	currentAnimation = &walkAnim;

	//Die
	dieAnim.PushBack({ 2, 61, 28, 13 });
	dieAnim.PushBack({ 34, 60, 28, 14 });
	dieAnim.PushBack({ 66, 58, 28, 16 });
	dieAnim.PushBack({ 96, 65, 30, 9 });
	dieAnim.PushBack({ 128, 72, 31, 2 });
	dieAnim.loop = false;
	dieAnim.speed = 0.02f;

	return true;
}

bool Enemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool Enemy::Update()
{
	currentAnimation->Update();
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x+8, position.y+8, &(currentAnimation->GetCurrentFrame()));

	return true;
}

bool Enemy::CleanUp()
{
	this->active = false;
	return true;
}