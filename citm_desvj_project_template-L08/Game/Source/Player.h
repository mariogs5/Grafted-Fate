#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	PhysBody* pbody;

	float speed = 3.0;
	float impulse = 7.0;
	int jumps = 0;

	bool Anim_right;
	bool GodMode;
	bool Alive;
	bool JumpAnim;

	Animation* currentAnimation;

	Animation idle_right;
	Animation idle_left;

	Animation run_right;
	Animation run_left;
	
	Animation jump_right;
	Animation jump_left;

	Animation death_right;
	Animation death_left;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	

	int pickCoinFxId;

};

#endif // __PLAYER_H__