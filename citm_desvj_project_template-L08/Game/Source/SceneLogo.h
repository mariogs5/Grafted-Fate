#pragma once

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "Audio.h"

struct SDL_Texture;

class SceneLogo : public Module
{
public:

	SceneLogo(bool StartEnabled);

	// Destructor
	virtual ~SceneLogo();

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

	GuiButton* Play;

	uint Jovani;

private:

	SDL_Texture* img;

};
