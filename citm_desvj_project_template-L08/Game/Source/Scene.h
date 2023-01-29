#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "List.h"
#include "Enemy.h"
#include "GuiButton.h"
#include "Audio.h"

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
	List<Item*> Items;

	SDL_Texture* Fondo = nullptr;
	iPoint playertile;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	bool MenuOn;
	bool SettingsOn;

	int Health;
	int Score;


	GuiButton* Mslider;
	GuiButton* SFXslider;


private:

	SDL_Texture* img;

	//Debug pathfing
	iPoint origin;
	bool originSelected = false;

	//UI
	//Player HUD

	SDL_Texture* CollectiblesIncon;
	SDL_Rect health;
	

	SDL_Texture* HealthIcon;


	//Buttons
	SDL_Texture* MenuImage;
	SDL_Texture* SettingsImage;
	
	
	bool SincroV;
	bool Fullscreen;

	//Menu

	SDL_Texture* resume;
	GuiButton* Resume;

	SDL_Texture* settings;
	GuiButton* Settings;

	SDL_Texture* title;
	GuiButton* Title;

	SDL_Texture* exit;
	GuiButton* Exit;
	
	//Settings 

	SDL_Texture* back;
	GuiButton* Back;

	SDL_Texture* Slider;


	SDL_Texture* Box;

	GuiButton* Vsync;
	GuiButton* Full;

	SDL_Texture* Check;
};

#endif // __SCENE_H__