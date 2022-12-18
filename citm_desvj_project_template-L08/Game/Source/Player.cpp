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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

}

Player::~Player() {

}

bool Player::Awake() {

	//Define animations

	//IDLE RIGHT ANIMATION

	for (int i = 0; i < 2; i++) {

		idle_right.PushBack({ 32 * (0 + i), 32 * 0, 32, 32 });

	}
	idle_right.loop = true;
	idle_right.speed = 0.03f;

	//IDLE LEFT ANIMATION

	for (int i = 7; i >= 6; i--) {

		idle_left.PushBack({ 32 * (0 + i), 32 * 4, 32, 32 });

	}
	idle_left.loop = true;
	idle_left.speed = 0.03f;

	//JUMP RIGHT ANIMATION

	for (int i = 0; i < 3; i++) {

		jump_right.PushBack({ 32 * (0 + i), 32 * 1, 32, 32 });

	}
	jump_right.loop = false;
	jump_right.speed = 0.1f;

	//JUMP LEFT ANIMATION

	for (int i = 7; i >= 5; i--) {

		jump_left.PushBack({ 32 * (0 + i), 32 * 5, 32, 32 });

	}
	jump_left.loop = false;
	jump_left.speed = 0.1f;

	//RUN RIGHT ANIMATION

	for (int i = 0; i < 8; i++) {

		run_right.PushBack({ 32 * (0 + i), 32 * 3, 32, 32 });

	}
	run_right.loop = true;
	run_right.speed = 0.1f;

	//RUN LEFT ANIMATION

	for (int i = 7; i >= 0; i--) {

		run_left.PushBack({ 32 * (0 + i), 32 * 7, 32, 32 });

	}
	run_left.loop = true;
	run_left.speed = 0.1f;

	//DEATH RIGHT ANIMATION

	for (int i = 0; i < 7; i++) {

		death_right.PushBack({ 32 * (0 + i), 32 * 2, 32, 32 });

	}
	death_right.loop = false;
	death_right.speed = 0.1f;

	//DEATH LEFT ANIMATION

	for (int i = 7; i >= 1; i--) {

		death_left.PushBack({ 32 * (0 + i), 32 * 6, 32, 32 });

	}
	death_left.loop = false;
	death_left.speed = 0.1f;


	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	Anim_right = true;
	GodMode = false;
	Alive = true;
	JumpAnim = false;

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 15, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//Animacion por la que empieza

	currentAnimation = &idle_right;

	return true;
}

bool Player::Update()
{
	// L07 DONE 5: Add physics to the player - updated player position using physics
	// 
	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	
	if (Anim_right) {

		currentAnimation = &idle_right;
	}
	else {

		currentAnimation = &idle_left;
	}

	//DEBUG KEYS

	//Load 1rst level start
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {

		app->render->camera.x = 0;
		app->render->camera.y = 0;
		app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(32),PIXEL_TO_METERS(820) }, 0);
	}

	//Load actual level start
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {

		app->render->camera.x = 0;
		app->render->camera.y = 0;
		app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(32),PIXEL_TO_METERS(820) }, 0);
	}

	//God mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && !GodMode) {

		GodMode = true;		
	}
	else if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && GodMode) {

		GodMode = false;
	}

	
	//PLAYER MOVEMENT
	if (!GodMode) {

		b2Vec2 velocity = { 0, pbody->body->GetLinearVelocity().y };

		if (Alive) {

			if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {

				Alive = false;

			}
			
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

				velocity = { -speed, pbody->body->GetLinearVelocity().y };
				currentAnimation = &run_left;
				Anim_right = false;
			}

			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

				velocity = { speed, pbody->body->GetLinearVelocity().y };
				currentAnimation = &run_right;
				Anim_right = true;
			}

			if (jumps < 2) {

				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

					velocity = { pbody->body->GetLinearVelocity().x, -impulse };
					jumps++;
					JumpAnim = true;

				}
			}

			if (JumpAnim) {

				if (Anim_right) {

					currentAnimation = &jump_right;

					if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

						currentAnimation->Reset();
						currentAnimation = &jump_right;
					}
				}

				else {

					currentAnimation = &jump_left;

					if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

						currentAnimation->Reset();
						currentAnimation = &jump_left;
					}
				}
			}
		}

		//Death animation

		if (!Alive) {

			if (Anim_right) {

				currentAnimation = &death_right;

				if (currentAnimation->HasFinished()) {

					app->fade->Fade((Module*)app->scene, (Module*)app->scenedeath, 0);
					
					currentAnimation->Reset();
				}

				if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && currentAnimation->HasFinished()) {

					Alive = true;
				}
			}
			else {

				currentAnimation = &death_left;

				if (currentAnimation->HasFinished()) {

					app->fade->Fade((Module*)app->scene, (Module*)app->scenedeath, 0);
					
				}

				if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && currentAnimation->HasFinished()) {

					currentAnimation->Reset();
					Alive = true;
				}
			}
			
		}
	
		//Set the velocity of the pbody of the player
		pbody->body->SetLinearVelocity(velocity);

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	}
	else {

		//CAMERA MOVEMENT IN GODMODE
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

			position.x -= 10;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

			position.x += 10;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

			position.y -= 10;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {

			position.y += 10;
		}
	}
	

	//Update player position in pixels

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y , &rect);

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
		
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			jumps = 0;
			JumpAnim = false;
			break;

		case ColliderType::CEILING:
			LOG("Collision CEILING");
			break;

		case ColliderType::WALL:
			LOG("Collision WALL");
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
			app->audio->PlayFx(pickCoinFxId);
			break;
	}
	


}
