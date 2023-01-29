#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "FadeToBlack.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Timer.h"
#include "Enemy.h"
#include "Pathfinding.h"
#include "Map.h"
#include "SceneTitle.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiControl.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");

}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//Walking Enemy animations

	//Idle right animation
	for (int i = 0; i < 1; i++) {

		idle_right_walking.PushBack({52 * (0 + i), 32 * 6, 52, 32 });

	}
	idle_right_walking.loop = false;
	idle_right_walking.speed = 0.03f;

	//Idle left animation
	for (int i = 7; i >= 7; i--) {

		idle_left_walking.PushBack({ 52 * (0 + i), 32 * 13, 52, 32 });

	}
	idle_left_walking.loop = false;
	idle_left_walking.speed = 0.03f;




	//Run right animation

	for (int i = 0; i < 8; i++) {

		run_right_walking.PushBack({ 52 * (0 + i), 32 * 0, 52, 32 });

	}
	run_right_walking.loop = true;
	run_right_walking.speed = 0.08f;

	//Run left animation

	for (int i = 7; i >= 0; i--) {

		run_left_walking.PushBack({ 52 * (0 + i), 32 * 7, 52, 32 });

	}
	run_left_walking.loop = true;
	run_left_walking.speed = 0.1f;




	//attack right animation

	for (int i = 0; i < 4; i++) {

		attack_right_walking.PushBack({ 52 * (0 + i), 32 * 2, 52, 32 });

	}
	attack_right_walking.loop = false;
	attack_right_walking.speed = 0.03f;

	//attack left animation

	for (int i = 7; i >= 4; i--) {

		attack_left_walking.PushBack({ 52 * (0 + i), 32 * 9, 52, 32 });

	}
	attack_left_walking.loop = false;
	attack_left_walking.speed = 0.03f;




	//Death right animation

	for (int i = 0; i < 6; i++) {

		death_right_walking.PushBack({ 52 * (0 + i), 32 * 1, 52, 32 });

	}
	death_right_walking.loop = false;
	death_right_walking.speed = 0.03f;



	//Death left animation

	for (int i = 7; i >= 2; i--) {

		death_left_walking.PushBack({ 52 * (0 + i), 32 * 8, 52, 32 });

	}
	death_left_walking.loop = false;
	death_left_walking.speed = 0.03f;



	//Flying Enemy Animations
	
	//Idle right animation
	for (int i = 0; i < 6; i++) {

		idle_right_flying.PushBack({ 64 * (0 + i), 64 * 0, 64, 64 });

	}
	idle_right_flying.loop = true;
	idle_right_flying.speed = 0.1f;

	//Idle left animation
	for (int i = 11; i >= 6; i--) {

		idle_left_flying.PushBack({ 64 * (0 + i), 64 * 3, 64, 64 });

	}
	idle_left_flying.loop = true;
	idle_left_flying.speed = 0.1f;




	//attack right animation

	for (int i = 0; i < 12; i++) {

		attack_right_flying.PushBack({ 64 * (0 + i), 64 * 1, 64, 64 });

	}
	attack_right_flying.loop = false;
	attack_right_flying.speed = 0.03f;

	//attack left animation

	for (int i = 11; i >= 0; i--) {

		attack_left_flying.PushBack({ 64 * (0 + i), 64 * 3, 64, 64 });

	}
	attack_left_flying.loop = false;
	attack_left_flying.speed = 0.03f;




	//Death right animation

	for (int i = 0; i < 7; i++) {

		death_right_flying.PushBack({ 64 * (0 + i), 64 * 2, 64, 64 });

	}
	death_right_flying.loop = false;
	death_right_flying.speed = 0.1f;



	//Death left animation

	for (int i = 11; i >= 5; i--) {

		death_left_flying.PushBack({ 64 * (0 + i), 64 * 5, 64, 64 });

	}
	death_left_flying.loop = false;
	death_left_flying.speed = 0.1f;


	Anim_right = true;
	Alive = true;

	if (SString(parameters.attribute("type").as_string()) == SString("walking"))
		type = EnemyType::WALKING;
	if (SString(parameters.attribute("type").as_string()) == SString("flying"))
		type = EnemyType::FLYING;

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	initposx = parameters.attribute("x").as_int();
	initposy = parameters.attribute("y").as_int();
	pos1= parameters.attribute("pos1").as_int();
	pos2 = parameters.attribute("pos2").as_int();

	return true;
}

bool Enemy::Start() {

	if (type == EnemyType::WALKING) {

		texture = app->tex->Load(texturePath);
		pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 15, bodyType::DYNAMIC);
		pbody->listener = this;
		pbody->ctype = ColliderType::ENEMY;
		Detectpbody = app->physics->CreateCircleSensor(position.x, position.y, 100, bodyType::KINEMATIC, ColliderType::DETECTION);
		Detectpbody->listener = this;
	
		currentAnimation = &run_right_walking;
	}

	else {

		texture = app->tex->Load(texturePath);
		pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 15, bodyType::KINEMATIC);
		pbody->listener = this;
		pbody->ctype = ColliderType::ENEMY;
		Detectpbody = app->physics->CreateCircleSensor(position.x, position.y , 100, bodyType::KINEMATIC, ColliderType::DETECTION);
		Detectpbody->listener = this;

		currentAnimation = &run_right_flying;
	}

	path = nullptr;
	pathPosition = 1;
	timer.Start();
	detection = false;

	return true;
}

bool Enemy::Update()
{
	int enemyX, enemyY;
	pbody->GetPosition(enemyX, enemyY);

	origin = app->map->WorldToMap(enemyX, enemyY);

	app->pathfinding->CreatePath(origin, app->scene->playertile);

	path = app->pathfinding->GetLastPath();

	// L12: Get the latest calculated path and draw
	
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		if(app->physics->debug)app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y);
	}

	// L12: Debug pathfinding
	
	if (app->scenetitle->Play->state == GuiControlState::PRESSED) {

		Alive = true;
		pbody->body->SetTransform({ PIXEL_TO_METERS(initposx),PIXEL_TO_METERS(initposy) }, 0);
		Detectpbody->body->SetTransform({ PIXEL_TO_METERS(initposx),PIXEL_TO_METERS(initposy) }, 0);
		Anim_right = true;
	}

	if (timer.ReadMSec() > 1500) {

		if (pathPosition < path->Count()) {

			pathPosition++;

		}

		timer.Start();
	}


	b2Vec2 velocity = { 0, pbody->body->GetLinearVelocity().y };

	if (type == EnemyType::WALKING) {

		if (Alive) {
			
			if (detection) {
				
				if (pathPosition < path->Count()) {

					if (path->At(pathPosition)->x - path->At(pathPosition - 1)->x > 0) {

						currentAnimation = &run_right_walking;
						Anim_right = true;
						velocity = { speed, pbody->body->GetLinearVelocity().y };

					}

					if (path->At(pathPosition)->x - path->At(pathPosition - 1)->x < 0) {

						currentAnimation = &run_left_walking;
						Anim_right = false;
						velocity = { -speed, pbody->body->GetLinearVelocity().y };

					}
				}
				
			}
			else {

				pathPosition = 1;

				//timer para ver cuanto se mueve
				if (position.x < pos2 && Anim_right || position.x == pos1) {

					velocity = { speed, pbody->body->GetLinearVelocity().y };
					currentAnimation = &run_right_walking;
					Anim_right = true;
				}

				if (position.x > pos1 && !Anim_right || position.x == pos2) {

					velocity = { -speed, pbody->body->GetLinearVelocity().y };
					currentAnimation = &run_left_walking;
					Anim_right = false;
				}

			}
			
		}
		else {

			if (Anim_right) {

				currentAnimation = &death_right_walking;
			}
			else {

				currentAnimation = &death_left_walking;
			}
		}
	}
	
	if (type == EnemyType::FLYING) {

		if (Alive) {

			if (detection) {

				if (pathPosition < path->Count()) {

					if (path->At(pathPosition)->x - path->At(pathPosition - 1)->x > 0) {

						currentAnimation = &idle_right_flying;
						Anim_right = true;
						velocity = { speed,0 };

					}

					if (path->At(pathPosition)->x - path->At(pathPosition - 1)->x < 0) {

						currentAnimation = &idle_right_flying;
						Anim_right = false;
						velocity = { -speed,0 };

					}

					if (path->At(pathPosition)->y - path->At(pathPosition - 1)->y > 0) velocity = { 0,speed };
					if (path->At(pathPosition)->y - path->At(pathPosition - 1)->y < 0) velocity = { 0,-speed };
				}

			}
			else {

				pathPosition = 1;

				//timer para ver cuanto se mueve
				if (position.x < pos2 && Anim_right || position.x == pos1) {

					velocity = { speed, pbody->body->GetLinearVelocity().y };
					currentAnimation = &idle_right_flying;
					Anim_right = true;
				}

				if (position.x > pos1 && !Anim_right || position.x == pos2) {

					velocity = { -speed, pbody->body->GetLinearVelocity().y };
					currentAnimation = &idle_left_flying;
					Anim_right = false;

				}
			}
			
		}
		else {

			if (Anim_right) {

				currentAnimation = &death_right_flying;
			}
			else {

				currentAnimation = &death_left_flying;
			}
		}
	}

	pbody->body->SetLinearVelocity(velocity);
	Detectpbody->body->SetLinearVelocity(velocity);

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 25;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool Enemy::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{

	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;

	case ColliderType::CEILING:
		LOG("Collision CEILING");
		break;


	case ColliderType::WALL:
		LOG("Collision WALL");
		break;

	case ColliderType::ATTACK:
		LOG("Collision CEILING");
		break;

	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		detection = true;
		break;

	case ColliderType::KILL:
		LOG("Collision KILL");
		Alive = false;
		break;

	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;

	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	}
}


void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {

	detection = false;
}