#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheck.h"
#include "Audio.h"

GuiManager::GuiManager(bool StartEnabled) : Module(StartEnabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, SDL_Texture* tex, int x, int y, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	// L15: TODO1: Create a GUI control and add it to the list of controls

	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:

		guiControl = new GuiButton(id, tex, x, y, bounds, text);

		break;
	case GuiControlType::TOGGLE:

		break;
	case GuiControlType::CHECKBOX:

		guiControl = new GuiCheck(id, tex, x, y, bounds, text);

		break;
	case GuiControlType::SLIDER:

		guiControl = new GuiSlider(id, tex, x, y, bounds, text);

		break;
	case GuiControlType::SLIDERBAR:

		break;
	case GuiControlType::COMBOBOX:

		break;
	case GuiControlType::DROPDOWNBOX:

		break;
	case GuiControlType::INPUTBOX:

		break;
	case GuiControlType::VALUEBOX:

		break;
	case GuiControlType::SPINNER:

		break;
	}

	//Set the observer

	guiControl->SetObserver(observer);

	// Created GuiControls are add it to the list of controls

	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		ListItem<GuiControl*>* control = guiControlsList.start;

		while (control != nullptr)
		{
			control->data->Update(dt);
			control = control->next;
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Draw(app->render);
		control = control->next;
	}

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}