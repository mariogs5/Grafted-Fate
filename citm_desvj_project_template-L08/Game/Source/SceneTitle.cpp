#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Physics.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "GuiManager.h"
#include "GuiSlider.h"

#include "SDL_mixer/include/SDL_mixer.h"


#include "Defs.h"
#include "Log.h"

SceneTitle::SceneTitle(bool StartEnabled) : Module(StartEnabled)
{
	name.Create("scenetitle");
}

// Destructor
SceneTitle::~SceneTitle()
{}

// Called before render is available
bool SceneTitle::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneTitle");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneTitle::Start()
{

	SettingsOn = false;
	SincroV = false;
	Fullscreen = false;

	//Settings OFF

	Fondo = app->tex->Load("Assets/Textures/TitleScreen.png");
	SettingsImage = app->tex->Load("Assets/UI/SettingsImage.png");
	Check = app->tex->Load("Assets/UI/Check.png");

	settings = app->tex->Load("Assets/UI/Settings.png");
	Settings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, settings, 37, 529, "", { 37,529,284,48 }, this);
	
	play = app->tex->Load("Assets/UI/Play.png");
	Play = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, play, 435, 403, "", { 435,403,157,48 }, this);

	exit = app->tex->Load("Assets/UI/Exit.png");
	Exit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, exit, 779, 529, "", { 779,529,141,48 }, this);

	credits = app->tex->Load("Assets/UI/Credits.png");
	Credits = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, credits, 391, 529, "", { 391,529,244,48 }, this);

	//Settings ON

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

	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->render->DrawTexture(Fondo, 0, 0);
	app->entityManager->Disable();

	app->guiManager->Draw();



	if (Settings->state == GuiControlState::PRESSED)SettingsOn = true;
	if (Back->state == GuiControlState::PRESSED)SettingsOn = false;

	if (SettingsOn) {

		app->render->DrawTexture(SettingsImage, 194, 70);

		if (Settings->state != GuiControlState::DISABLED)Settings->state = GuiControlState::DISABLED;
		if (Play->state != GuiControlState::DISABLED)Play->state = GuiControlState::DISABLED;
		if (Exit->state != GuiControlState::DISABLED)Exit->state = GuiControlState::DISABLED;
		if (Credits->state != GuiControlState::DISABLED)Credits->state = GuiControlState::DISABLED;

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

		if (Settings->state == GuiControlState::DISABLED)Settings->state = GuiControlState::NORMAL;
		if (Play->state == GuiControlState::DISABLED)Play->state = GuiControlState::NORMAL;
		if (Exit->state == GuiControlState::DISABLED)Exit->state = GuiControlState::NORMAL;
		if (Credits->state == GuiControlState::DISABLED)Credits->state = GuiControlState::NORMAL;

	}

	app->scene->Mslider->bounds.x = Mslider->bounds.x;
	app->scene->Mslider->posx = Mslider->posx;

	app->scene->SFXslider->bounds.x = SFXslider->bounds.x;
	app->scene->SFXslider->posx = SFXslider->posx;

	Mix_VolumeMusic((Mslider->bounds.x - 380) * (128 - 0) / (622 - 380) + 0);

	for (int i = 0; i < app->audio->fx.Count(); i++) {

		Mix_VolumeChunk(app->audio->fx.At(i)->data, (Mslider->bounds.x - 380) * (128 - 0) / (622 - 380) + 0);

	}


	//Fade to black
	if (Play->state == GuiControlState::PRESSED) {

		app->scene->player->Alive = true;
		app->scene->Health = 4;
		app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(32),PIXEL_TO_METERS(820) }, 0);
		
		if (Settings->state != GuiControlState::DISABLED)Settings->state = GuiControlState::DISABLED;
		if (Play->state != GuiControlState::DISABLED)Play->state = GuiControlState::DISABLED;
		if (Exit->state != GuiControlState::DISABLED)Exit->state = GuiControlState::DISABLED;
		if (Credits->state != GuiControlState::DISABLED)Credits->state = GuiControlState::DISABLED;

		app->fade->Fade(this, (Module*)app->scene, 0);

	}

	
	return true;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	if (Exit->state == GuiControlState::PRESSED)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Freeing scene");

	return true;
}