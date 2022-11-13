#pragma once

#include "Module.h"
#include "Player.h"
#include "Item.h"

struct SDL_Texture;

class SceneDeath : public Module
{
public:

	SceneDeath(bool StartEnabled);

	// Destructor
	virtual ~SceneDeath();

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
	SDL_Texture* Fondo = nullptr;

private:
	SDL_Texture* img;

};