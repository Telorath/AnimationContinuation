#include "InputManager.h"
#include <Windows.h>
#include <map>

bool ButtonState[InputManager::Button::mouse_3 + 1];

bool ButtonDown[InputManager::Button::mouse_3 + 1];

bool ButtonUp[InputManager::Button::mouse_3 + 1];

POINT OldCursorPosition;
POINT NewCursorPosition;

void InputManager::RegisterKey(const char*, Button keybind)
{

}

void InputManager::setstate(Button Buttoncode, bool newstate)
{
	ButtonState[Buttoncode] = newstate;
	if (newstate)
	{
		ButtonDown[Buttoncode] = true;
	}
	else
	{
		ButtonUp[Buttoncode] = true;
	}
}

bool InputManager::getkeystate(Button Buttoncode)
{
	return ButtonState[Buttoncode];
}

bool InputManager::getkeydown(Button buttoncode)
{
	return ButtonDown[buttoncode];
}

void InputManager::update()
{
	OldCursorPosition = NewCursorPosition;
	GetCursorPos(&NewCursorPosition);
}

void InputManager::finalupdate()
{
	for (size_t i = 0; i <= mouse_3; i++)
	{
		ButtonUp[i] = false;
	}
	for (size_t i = 0; i <= mouse_3; i++)
	{
		ButtonDown[i] = false;
	}
}
void InputManager::GetCursorDifference(float* x, float* y)
{
	*x = (float)(NewCursorPosition.x - OldCursorPosition.x);
	*y = (float)(NewCursorPosition.y - OldCursorPosition.y);
}