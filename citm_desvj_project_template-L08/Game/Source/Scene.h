#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "List.h"
#include "Enemy.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool StartEnabled);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	List<Enemy*> Enemies;
	SDL_Texture* Fondo = nullptr;
	iPoint playertile;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

private:
	SDL_Texture* img;

	//Debug pathfing
	iPoint origin;
	bool originSelected = false;
};

#endif // __SCENE_H__