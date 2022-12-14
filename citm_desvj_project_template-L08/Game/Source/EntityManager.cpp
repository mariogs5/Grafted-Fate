#include "EntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(bool StartEnabled) : Module(StartEnabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L02: DONE 2: Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player();
		break;

	case EntityType::ENEMY:
		entity = new Enemy();
		break;

	case EntityType::ITEM:
		entity = new Item();
		break;

	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update();
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node& data) {

	float x = data.child("player").attribute("x").as_int();
	float y = data.child("player").attribute("y").as_int();

	app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(x),PIXEL_TO_METERS(y)}, 0);

	ListItem<Enemy*>* item;
	pugi::xml_node enemy = data.child("enemy");

	for (item = app->scene->Enemies.start; item != NULL; item = item->next, enemy = enemy.next_sibling("enemy")) {
		
		float x = enemy.attribute("x").as_int();
		float y = enemy.attribute("y").as_int();
		item->data->pbody->body->SetTransform({ PIXEL_TO_METERS(x),PIXEL_TO_METERS(y) }, 0);
		item->data->Detectpbody->body->SetTransform({ PIXEL_TO_METERS(x),PIXEL_TO_METERS(y) }, 0);

	}

	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) {

	pugi::xml_node player = data.append_child("player");

	ListItem<Enemy*>* item;
	
	for (item = app->scene->Enemies.start; item != NULL; item = item->next) {

		pugi::xml_node enemy = data.append_child("enemy");

		int posx, posy;
		item->data->pbody->GetPosition(posx, posy);
		
		enemy.append_attribute("x") = posx;
		enemy.append_attribute("y") = posy;

	}

	player.append_attribute("x") = app->scene->player->position.x;
	player.append_attribute("y") = app->scene->player->position.y;

	return true;
}