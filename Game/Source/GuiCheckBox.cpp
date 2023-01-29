#include "GuiCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiCheckBox::~GuiCheckBox()
{

}

bool GuiCheckBox::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN && state == GuiControlState::NORMAL) {
				state = GuiControlState::SELECTED;
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KeyState::KEY_DOWN && state == GuiControlState::SELECTED) {
				state = GuiControlState::NORMAL;
			}
			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
	}

	return false;
}




bool GuiCheckBox::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 200, 200, 200, 0, false, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 255, 159, 77, 255, false, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 195, 84, 28, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	case GuiControlState::SELECTED:
		render->DrawRectangle(bounds, 255, 159, 77, 255, true, false);
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, {255,255,255});

	return false;
}