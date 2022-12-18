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
#include "EntityManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

SceneLogo::SceneLogo(bool StartEnabled) : Module(StartEnabled)
{
	name.Create("scenelogo");
}

// Destructor
SceneLogo::~SceneLogo()
{}

// Called before render is available
bool SceneLogo::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneLogo");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLogo::Start()
{
	Fondo = app->tex->Load("Assets/Textures/StudioScreen.png");

	return true;
}

// Called each loop iteration
bool SceneLogo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLogo::Update(float dt)
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->entityManager->Disable();
	app->render->DrawTexture(Fondo, 0, 0);

	//Fade to black
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		app->fade->Fade(this, (Module*)app->scenetitle, 0);

	}

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;



	return ret;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing scene");

	return true;
}