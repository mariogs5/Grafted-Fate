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
	Fondo = app->tex->Load("Assets/Textures/TitleScreen.png");

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
	app->entityManager->Disable();
	app->render->DrawTexture(Fondo,0,128);

	//Fade to black
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		app->fade->Fade(this, (Module*)app->scene, dt);

	}
	
	return true;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	

	return ret;
}

// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Freeing scene");

	return true;
}