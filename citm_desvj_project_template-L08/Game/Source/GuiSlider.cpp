#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Window.h"

GuiSlider::GuiSlider(uint32 id, SDL_Texture* tex, int x, int y, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->tex = tex;
	this->posx = x;
	this->posy = y;

	canClick = true;
	drawBasic = false;

	this->x1 = 0;
	this->y1 = 0;
	this->x2 = 0;
	this->y2 = 0;

}

GuiSlider::~GuiSlider()
{

}

bool GuiSlider::Update(float dt)
{

	if (state != GuiControlState::DISABLED)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position


		int x0, y0;
		app->input->GetMousePosition(x0, y0);

		if (state == GuiControlState::FOCUSED) {

			app->input->GetMousePosition(x1, y1);

		}

		if (state == GuiControlState::PRESSED) {

			app->input->GetMousePosition(x2, y2);
		}

		if (x0 >= bounds.x && x0 <= bounds.x + bounds.w && y0 >= bounds.y && y0 <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {

				state = GuiControlState::PRESSED;

				if (posx >= 380 && posx <= 622) {

					bounds.x = x0 - bounds.w / 2;
					posx = x0 - bounds.w / 2;
				}
				if (posx <= 380) {

					posx = 380;

				}
				if (posx > 622) {

					posx = 621;
				}
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

bool GuiSlider::Draw(Render* render)
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