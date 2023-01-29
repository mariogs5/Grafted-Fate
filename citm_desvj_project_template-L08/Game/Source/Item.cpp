#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Player.h"
#include "Physics.h"
#include "Audio.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	Coin = app->audio->LoadFx("Assets/Audio/SFX/Collectible.wav");

	for (int i = 0; i < 3; i++) {

		CollectibleAnim.PushBack({ 32 * (0 + i), 0, 32, 32 });

	}
	CollectibleAnim.loop = true;
	CollectibleAnim.speed = 0.03f;

	for (int i = 0; i < 3; i++) {

		HealthAnim.PushBack({ 32 * (0 + i), 0, 32, 32 });

	}
	HealthAnim.loop = true;
	HealthAnim.speed = 0.03f;

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	initposx = parameters.attribute("x").as_int();
	initposy = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	if (SString(parameters.attribute("type").as_string()) == SString("collectible"))
		type = ItemType::COLL;
	if (SString(parameters.attribute("type").as_string()) == SString("flying"))
		type = ItemType::HEAL;

	return true;
}

bool Item::Start() {

	if (type == ItemType::COLL) {

		texture = app->tex->Load(texturePath);
		pbody = app->physics->CreateCircleSensor(position.x + 16, position.y + 16, 15, bodyType::KINEMATIC, ColliderType::COLLECTIBLE);
		pbody->listener = this;

		currentAnimation = &CollectibleAnim;
	}

	else {

		texture = app->tex->Load(texturePath);
		pbody = app->physics->CreateCircleSensor(position.x + 16, position.y + 16, 15, bodyType::KINEMATIC, ColliderType::HEALTH);
		pbody->listener = this;

		currentAnimation = &HealthAnim;

	}

	//pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);

	return true;
}

bool Item::Update()
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool Item::CleanUp()
{
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{

	case ColliderType::PLAYER:

		//SFX

		app->audio->PlayFx(Coin);

		if (app->scene->Health < 4) {

			app->scene->Health++;
		}

		app->scene->Score += 100;

		Disable();
		pbody->body->DestroyFixture(pbody->body->GetFixtureList());
	
		break;
	}
}