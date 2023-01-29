#pragma once
#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Textures.h"

class GuiCheck : public GuiControl
{
public:

	GuiCheck(uint32 id, SDL_Texture* tex, int x, int y, SDL_Rect bounds, const char* text);
	virtual ~GuiCheck();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;
	unsigned int Select;

	bool canClick = true;
	bool drawBasic = false;
	SDL_Texture* tex;
	SDL_Rect rect;
	bool crossed;
	int x;
	int y;
	int posx;
	int posy;

};
