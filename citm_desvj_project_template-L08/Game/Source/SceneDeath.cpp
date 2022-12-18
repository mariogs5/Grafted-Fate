#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "Scene.h"
#include "SceneDeath.h"
#include "EntityManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

SceneDeath::SceneDeath(bool StartEnabled) : Module(StartEnabled)
{
	name.Create("scenedeath");
}

// Destructor
SceneDeath::~SceneDeath()
{}

// Called before render is available
bool SceneDeath::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneDeath");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneDeath::Start()
{
	Fondo = app->tex->Load("Assets/Textures/DeathScreen.png");

	return true;
}

// Called each loop iteration
bool SceneDeath::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneDeath::Update(float dt)
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->entityManager->Disable();
	app->render->DrawTexture(Fondo, 0, 0);

	//Fade to black
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		app->fade->Fade(this, (Module*)app->scenelogo, 0);

	}

	return true;
}

// Called each loop iteration
bool SceneDeath::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;



	return ret;
}

// Called before quitting
bool SceneDeath::CleanUp()
{
	LOG("Freeing scene");

	return true;
}