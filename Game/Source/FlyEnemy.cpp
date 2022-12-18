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
#include "Pathfinding.h"
#include "Map.h"

FlyEnemy::FlyEnemy() : Entity(EntityType::FLY_ENEMY)
{
	name.Create("FlyEnemy");
}

FlyEnemy::~FlyEnemy() {}

bool FlyEnemy::Awake() {

	this->active = false;

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
	idleAnim.speed = 0.2f;
	currentAnimation = &idleAnim;

	//Flying
	flyAnim.PushBack({ 562, 7, 51, 43 });
	flyAnim.PushBack({ 623, 22, 52, 20 });
	flyAnim.PushBack({ 687, 18, 51, 23 });
	flyAnim.PushBack({ 623, 22, 52, 20 });
	flyAnim.PushBack({ 562, 7, 51, 43 });
	flyAnim.loop = true;
	flyAnim.speed = 0.1f;

	return true;
}

bool FlyEnemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	//joint
	view = app->physics->CreateCircleSensor(position.x + 100, position.y + 100, 200, bodyType::DYNAMIC);
	view->ctype = ColliderType::ENEMY_VIEW_FLY;

	b2RevoluteJointDef view_joint;
	view_joint.bodyA = pbody->body;
	view_joint.bodyB = view->body;
	view_joint.localAnchorA.Set(0, 0);
	view_joint.localAnchorB.Set(0, 0);
	b2RevoluteJoint* view_ = (b2RevoluteJoint*)app->physics->world->CreateJoint(&view_joint);

	view2 = app->physics->CreateRectangle(position.x + 100, position.y + 100, 20, 6, bodyType::DYNAMIC);
	view2->ctype = ColliderType::DIE_FLY_ENEMY;

	view2->body->SetFixedRotation(true);

	b2RevoluteJointDef view_joint2;
	view_joint2.bodyA = pbody->body;
	view_joint2.bodyB = view2->body;
	view_joint2.localAnchorA.Set(0,0);
	view_joint2.localAnchorB.Set(0, PIXEL_TO_METERS(10));
	b2RevoluteJoint* view_2 = (b2RevoluteJoint*)app->physics->world->CreateJoint(&view_joint2);

	mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/path_square.png");

	return true;
}

bool FlyEnemy::Update()
{
	currentAnimation->Update();
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	if (chasing == true) {
		iPoint pos_or = app->map->WorldToMap(position.x, position.y);
		iPoint pos_des = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y+32);

		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, pos_des);
			originSelected = false;
			if (origin.x < pos_des.x) {
				view->body->SetLinearVelocity(b2Vec2(1, 0));
				flip = SDL_RendererFlip::SDL_FLIP_NONE;
			}
			if (origin.x > pos_des.x) {
				view->body->SetLinearVelocity(b2Vec2(-1, 0));
				flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
			}
			if (origin.y < pos_des.y) {
				view->body->SetLinearVelocity(b2Vec2(0, 1));
			}
			if (origin.y > pos_des.y) {
				view->body->SetLinearVelocity(b2Vec2(0, -1));
			}
		}
		else
		{
			origin = pos_or;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
		currentAnimation = &flyAnim;

		if (app->physics->debug)
		{
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
	}
	else {
		currentAnimation = &idleAnim;
		view->body->SetLinearVelocity(b2Vec2(0, -0.3));
		app->pathfinding->ClearLastPath();
	}

	app->render->DrawTexture(texture, position.x + 8, position.y + 8, &(currentAnimation->GetCurrentFrame()), 1.0f, NULL, NULL, NULL, flip);

	position.x = METERS_TO_PIXELS(view->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(view->body->GetTransform().p.y) - 16;
	return true;
}

bool FlyEnemy::CleanUp()
{
	delete pbody;
	pbody = NULL;
	delete view;
	view = NULL;
	delete view2;
	view2 = NULL;
	this->active = false;
	return true;
}