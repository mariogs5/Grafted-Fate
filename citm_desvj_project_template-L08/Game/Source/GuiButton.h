#pragma once
#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Textures.h"

class GuiButton : public GuiControl
{
public:

	//Fx

	GuiButton(uint32 id, SDL_Texture* tex, int x, int y, SDL_Rect bounds, const char* text);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
	SDL_Texture* tex;
	SDL_Rect rect;

	int x;
	int y;
	int posx;
	int posy;

};
