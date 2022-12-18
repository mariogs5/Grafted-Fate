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
#include "Physics.h"
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
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->entityManager->Disable();
	app->render->DrawTexture(Fondo,0,0);

	

	//Fade to black
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		app->scene->player->Alive = true;
		app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(32),PIXEL_TO_METERS(820) }, 0);
	

		app->fade->Fade(this, (Module*)app->scene, 0);

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