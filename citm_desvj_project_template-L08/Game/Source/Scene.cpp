#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "Pathfinding.h"
#include "SceneTitle.h"
#include "Physics.h"
#include "GuiManager.h"

#include "Audio.h"
#include "SDL_mixer/include/SDL_mixer.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool StartEnabled) : Module(StartEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	for (pugi::xml_node enemyNode = config.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
	{
		Enemy* enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = enemyNode;
		Enemies.Add(enemy);
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	//UI

	//Player HUD
	//HealthIcon = app->tex->Load("Assets/Textures/Health bar.png");
	HealthIcon = app->tex->Load("Assets/Textures/Frame 17.png");
	Health = 4;
	health.x = 0;
	health.y = 0;
	health.w = 384;
	health.h = 96;

	CollectiblesIncon = app->tex->Load("Assets/Textures/StaticCollectible.png");


	//Menu

	MenuOn = false;
	SettingsOn = false;

	//Settings OFF

	MenuImage = app->tex->Load("Assets/UI/Menu.png");

	resume = app->tex->Load("Assets/UI/Resume.png");
	Resume = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, resume, 434, 253, "", { 434,253,159,37 }, this);
	Resume->state = GuiControlState::DISABLED;

	settings = app->tex->Load("Assets/UI/SettingsP.png");
	Settings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, settings, 421, 342, "", { 421,342,190,37 }, this);
	Settings->state = GuiControlState::DISABLED;

	title = app->tex->Load("Assets/UI/BackTitle.png");
	Title = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, title, 371, 431, "", { 371,431,290,37 }, this);
	Title->state = GuiControlState::DISABLED;

	exit = app->tex->Load("Assets/UI/ExitP.png");
	Exit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, exit, 467, 520, "", { 467,520,98,37 }, this);
	Exit->state = GuiControlState::DISABLED;

	//Settings ON

	SettingsImage = app->tex->Load("Assets/UI/SettingsImage.png");

	back = app->tex->Load("Assets/UI/Arrow.png");
	Back = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, back, 234, 120, "", { 234,120,56,61 }, this);
	Back->state = GuiControlState::DISABLED;

	Slider = app->tex->Load("Assets/UI/Slide.png");

	Mslider = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, Slider, 380, 281, "", { 380,281,22,56 }, this);
	Mslider->state = GuiControlState::DISABLED;

	SFXslider = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, Slider, 380, 413, "", { 380,413,22,56 }, this);
	SFXslider->state = GuiControlState::DISABLED;

	Box = app->tex->Load("Assets/UI/Box.png");
	Vsync = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, Box, 679, 498, "", { 679,498,56,50 }, this);
	Vsync->state = GuiControlState::DISABLED;

	Full = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, Box, 679, 578, "", { 679,578,56,50 }, this);
	Full->state = GuiControlState::DISABLED;

	
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	bool retLoad = app->map->Load();

	playertile = iPoint(0, 0);

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {

		// Texture to highligh mouse position 
		mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

		// Texture to show path origin 
		originTex = app->tex->Load("Assets/Maps/x_square.png");
	}

	Fondo = app->tex->Load("Assets/Maps/Fondo.png");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	if (!app->entityManager->IsEnabled()) {

		app->entityManager->Enable();

	}

	//Fade to black
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		app->scene->MenuOn = false;
		app->scene->SettingsOn = false;
		app->fade->Fade(this, (Module*)app->scenedeath, 0);

	}

	if (Title->state == GuiControlState::PRESSED) {

		if (Resume->state != GuiControlState::DISABLED)Resume->state = GuiControlState::DISABLED;
		if (Settings->state != GuiControlState::DISABLED)Settings->state = GuiControlState::DISABLED;
		if (Title->state != GuiControlState::DISABLED)Title->state = GuiControlState::DISABLED;
		if (Exit->state != GuiControlState::DISABLED)Exit->state = GuiControlState::DISABLED;

		app->scene->MenuOn = false;
		app->scene->SettingsOn = false;
		app->fade->Fade(this, (Module*)app->scenetitle, 0);
	}


	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	/*if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;*/

	float speed = 1 * dt;
	LOG("speed: %f dt: %f", speed, dt);
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= ceil(speed);

	// Draw map
	app->render->DrawTexture(Fondo, 0, 0, NULL, 1.1);
	app->map->Draw();

	int playerX, playerY;
	
	player->pbody->GetPosition(playerX, playerY);

	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {

		playertile = app->map->WorldToMap(playerX, playerY);

	}

	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(playertile.x, playertile.y);
	if (app->physics->debug) app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	app->pathfinding->CreatePath(origin, playertile);

	//UI

	switch (Health) {

	case 0:
		health.y = 96 * 4;
		app->render->DrawTexture(HealthIcon, -app->render->camera.x + 40, -app->render->camera.y + 20, &health);
		app->scene->player->Alive = false;
		break;

	case 1:
		health.y = 96 * 3;
		app->render->DrawTexture(HealthIcon, -app->render->camera.x + 40, -app->render->camera.y + 20, &health);
		break;

	case 2:
		health.y = 96 * 2;
		app->render->DrawTexture(HealthIcon, -app->render->camera.x + 40, -app->render->camera.y + 20, &health);
		break;

	case 3:
		health.y = 96 * 1;
		app->render->DrawTexture(HealthIcon, -app->render->camera.x + 40, -app->render->camera.y + 20, &health);
		break;

	case 4:
		health.y = 0;
		app->render->DrawTexture(HealthIcon, -app->render->camera.x + 40, -app->render->camera.y + 20, &health);
		break;
	}


	//app->guiManager->Draw();

	if ((app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN))MenuOn = true;

	if (Settings->state == GuiControlState::PRESSED) {
		SettingsOn = true;
		MenuOn = false;
	}
	if (Resume->state == GuiControlState::PRESSED) {
		MenuOn = false;
	}


	app->scenetitle->Mslider->bounds.x = Mslider->bounds.x;
	app->scenetitle->Mslider->posx = Mslider->posx;

	app->scenetitle->SFXslider->bounds.x = SFXslider->bounds.x;
	app->scenetitle->SFXslider->posx = SFXslider->posx;

	Mix_VolumeMusic((Mslider->bounds.x - 380) * (128 - 0) / (622 - 380) + 0);

	for (int i = 0; i < app->audio->fx.Count(); i++) {

		Mix_VolumeChunk(app->audio->fx.At(i)->data, (Mslider->bounds.x - 380) * (128 - 0) / (622 - 380) + 0);

	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	//Print UI

	if (MenuOn) {

		app->render->DrawTexture(MenuImage, -app->render->camera.x + 194, -app->render->camera.y + 70);

		if (Resume->state == GuiControlState::DISABLED)Resume->state = GuiControlState::NORMAL;
		if (Settings->state == GuiControlState::DISABLED)Settings->state = GuiControlState::NORMAL;
		if (Title->state == GuiControlState::DISABLED)Title->state = GuiControlState::NORMAL;
		if (Exit->state == GuiControlState::DISABLED)Exit->state = GuiControlState::NORMAL;

		Resume->Draw(app->render);
		Settings->Draw(app->render);
		Title->Draw(app->render);
		Exit->Draw(app->render);


	}
	else {

		if (Resume->state != GuiControlState::DISABLED)Resume->state = GuiControlState::DISABLED;
		if (Settings->state != GuiControlState::DISABLED)Settings->state = GuiControlState::DISABLED;
		if (Title->state != GuiControlState::DISABLED)Title->state = GuiControlState::DISABLED;
		if (Exit->state != GuiControlState::DISABLED)Exit->state = GuiControlState::DISABLED;
	}


	if (SettingsOn) {

		app->render->DrawTexture(SettingsImage, -app->render->camera.x + 194, -app->render->camera.y + 70);

		if (Back->state == GuiControlState::PRESSED)SettingsOn = false;

		if (Back->state == GuiControlState::DISABLED)Back->state = GuiControlState::NORMAL;
		if (Mslider->state == GuiControlState::DISABLED)Mslider->state = GuiControlState::NORMAL;
		if (SFXslider->state == GuiControlState::DISABLED)SFXslider->state = GuiControlState::NORMAL;
		if (Vsync->state == GuiControlState::DISABLED)Vsync->state = GuiControlState::NORMAL;
		if (Full->state == GuiControlState::DISABLED)Full->state = GuiControlState::NORMAL;

		Back->Draw(app->render);
		Mslider->Draw(app->render);
		SFXslider->Draw(app->render);
		Vsync->Draw(app->render);
		Full->Draw(app->render);
	}
	else {

		if (Back->state != GuiControlState::DISABLED)Back->state = GuiControlState::DISABLED;
		if (Mslider->state != GuiControlState::DISABLED)Mslider->state = GuiControlState::DISABLED;
		if (SFXslider->state != GuiControlState::DISABLED)SFXslider->state = GuiControlState::DISABLED;
		if (Vsync->state != GuiControlState::DISABLED)Vsync->state = GuiControlState::DISABLED;
		if (Full->state != GuiControlState::DISABLED)Full->state = GuiControlState::DISABLED;

	}

	if(Exit->state == GuiControlState::PRESSED )
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
