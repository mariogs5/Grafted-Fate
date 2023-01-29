#pragma once

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiButton.h"
#include "Audio.h"

struct SDL_Texture;

class SceneTitle : public Module
{
public:

	SceneTitle(bool StartEnabled);

	// Destructor
	virtual ~SceneTitle();

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

	bool SettingsOn;

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	SDL_Texture* Fondo = nullptr;
	pugi::xml_node parameters;

	SDL_Texture* play;
	GuiButton* Play;

	GuiButton* Mslider;
	GuiButton* SFXslider;

private:
	SDL_Texture* img;

	bool SincroV;
	bool Fullscreen;

	SDL_Texture* SettingsImage;

	SDL_Texture* exit;
	GuiButton* Exit;

	SDL_Texture* credits;
	GuiButton* Credits;

	SDL_Texture* settings;
	GuiButton* Settings;



	SDL_Texture* back;
	GuiButton* Back;

	SDL_Texture* Slider;

	SDL_Texture* Box;

	GuiButton* Vsync;
	GuiButton* Full;

	SDL_Texture* Check;


};
