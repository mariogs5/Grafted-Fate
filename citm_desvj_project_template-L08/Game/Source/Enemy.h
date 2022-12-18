#pragma once
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include"Timer.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

enum class EnemyType
{
	FLYING,
	WALKING,

};

class Enemy : public Entity
{
public:

	Enemy();

	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);


public:

	float speed = 1.0;
	float impulse = 7.0;
	int initposx;
	int initposy;
	int pos1;
	int pos2;

	bool detection;

	iPoint origin;
	const DynArray<iPoint>* path;
	Timer timer;
	int pathPosition;

	PhysBody* pbody;
	PhysBody* Detectpbody;

	EnemyType type;

	Animation* currentAnimation;

	// Walking enemy animations

	Animation idle_right_walking;
	Animation idle_left_walking;

	Animation run_right_walking;
	Animation run_left_walking;

	Animation attack_right_walking;
	Animation attack_left_walking;

	Animation death_right_walking;
	Animation death_left_walking;

	// Flying enemy animations

	Animation idle_right_flying;
	Animation idle_left_flying;

	Animation run_right_flying;
	Animation run_left_flying;

	Animation attack_right_flying;
	Animation attack_left_flying;

	Animation death_right_flying;
	Animation death_left_flying;

	bool Anim_right;
	bool Alive;
	

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body


	int pickCoinFxId;

};
