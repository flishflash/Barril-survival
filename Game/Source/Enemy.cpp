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
#include "Pathfinding.h"
#include "Map.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");
}

Enemy::~Enemy() {}

bool Enemy::Awake() {

	this->active = false;

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

	//joint
	view = app->physics->CreateCircleSensor(position.x + 100, position.y + 100, 150, bodyType::DYNAMIC);
	view->ctype = ColliderType::ENEMY_VIEW;

	b2RevoluteJointDef view_joint;
	view_joint.bodyA = pbody->body;
	view_joint.bodyB = view->body;
	view_joint.localAnchorA.Set(0, 0);
	view_joint.localAnchorB.Set(0, 0);
	b2RevoluteJoint* view_ = (b2RevoluteJoint*)app->physics->world->CreateJoint(&view_joint);

	view2 = app->physics->CreateRectangleSensor(position.x + 100, position.y + 100, 6, 20, bodyType::DYNAMIC);
	view2->ctype = ColliderType::DIE_ENEMY;

	b2RevoluteJointDef view_joint2;
	view_joint2.bodyA = pbody->body;
	view_joint2.bodyB = view2->body;
	view_joint2.localAnchorA.Set(0, 0);
	view_joint2.localAnchorB.Set(0, 0);
	b2RevoluteJoint* view_2 = (b2RevoluteJoint*)app->physics->world->CreateJoint(&view_joint2);

	mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/path_square.png");

	return true;
}

bool Enemy::Update()
{
	currentAnimation->Update();
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x+8, position.y+8, &(currentAnimation->GetCurrentFrame()));

	if (chasing == true) {
		iPoint pos_or = app->map->WorldToMap(position.x, position.y);
		iPoint pos_des = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, pos_des);
			originSelected = false;
			if (origin.x < pos_des.x) {
				view->body->SetLinearVelocity(b2Vec2(1, 0));
			}
			if (origin.x > pos_des.x) {
				view->body->SetLinearVelocity(b2Vec2(-1, 0));
			}
		}
		else
		{
			origin = pos_or;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			LOG("%d %d", path->At(i)->x, path->At(i)->y);
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}
		// L12: Debug pathfinding
		iPoint originScreen = app->map->MapToWorld(position.x, position.y);
		app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
		LOG("%d %d", originScreen.x, originScreen.y);

	}
	else {
		view->body->SetLinearVelocity(b2Vec2(0, 0));
		app->pathfinding->ClearLastPath();
	}

	return true;
}

bool Enemy::CleanUp()
{
	this->active = false;
	return true;
}
