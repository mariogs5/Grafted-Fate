#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum class ItemType
{
	COLL,
	HEAL,

};

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;
	ItemType type;
	
private:

	uint Coin;

	SDL_Texture* texture;
	const char* texturePath;
	SDL_Rect health;

	Animation* currentAnimation;

	Animation CollectibleAnim;
	Animation HealthAnim;

	int initposx;
	int initposy;

	//DONE 4: Add a physics to an item
	PhysBody* pbody;
};

#endif // __ITEM_H__