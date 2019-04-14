#include "InputManager.h"

uint8_t InputManager::keys[MAXKEYS] = { 0 };
uint8_t InputManager::mousebuttons[MAXMOUSEBUTTONS] = { 0 };
double InputManager::oldmouseposition[2] = { 0 };
double InputManager::newmouseposition[2] = { 0 };
double InputManager::deltamouseposition[2] = { 0 };

void InputManager::key_Callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	keys[key] = action;
}

void InputManager::mousebutton_Callback(GLFWwindow * window, int button, int action, int mods)
{
	mousebuttons[button] = action;
}

void InputManager::mouseposition_Callback(GLFWwindow * window, double xpos, double ypos)
{
	newmouseposition[0] = xpos;
	newmouseposition[1] = ypos;
	deltamouseposition[0] = newmouseposition[0] - oldmouseposition[0];
	deltamouseposition[1] = newmouseposition[1] - oldmouseposition[1];
	oldmouseposition[0] = newmouseposition[0];
	oldmouseposition[1] = newmouseposition[1];
}
