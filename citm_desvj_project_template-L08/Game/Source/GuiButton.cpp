#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiButton::GuiButton(uint32 id, SDL_Texture* tex, int x, int y, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->tex = tex;
	this->posx = x;
	this->posy = y;

	canClick = true;
	drawBasic = false;
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{

	if (state != GuiControlState::DISABLED)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position


		int x, y;
		app->input->GetMousePosition(x, y);

		if (x >= bounds.x && x <= bounds.x + bounds.w && y >= bounds.y && y <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
		

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {

				state = GuiControlState::PRESSED;
				
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {

				NotifyObserver();
			}
		}
		else {

			state = GuiControlState::NORMAL;
		}
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{

	rect.x = 0;
	rect.y = 0;
	rect.w = bounds.w;
	rect.h = bounds.h;

	//L15: TODO 4: Draw the button according the GuiControl State
	switch (state)
	{
	case GuiControlState::DISABLED:
		//app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:

		app->render->DrawTexture(tex, -app->render->camera.x + posx, -app->render->camera.y + posy, &rect);

		break;
	case GuiControlState::FOCUSED:

		rect.y = bounds.h;
		app->render->DrawTexture(tex, -app->render->camera.x + posx, -app->render->camera.y + posy, &rect);

		break;
	case GuiControlState::PRESSED:

		rect.y = bounds.h * 2;
		app->render->DrawTexture(tex, -app->render->camera.x + posx, -app->render->camera.y + posy, &rect);
		
		break;
	case GuiControlState::SELECTED:
		break;
	default:
		break;
	}


	return false;
}