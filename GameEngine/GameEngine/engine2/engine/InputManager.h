#pragma once
#include<Api.h>
#include<GLFW/glfw3.h>
#include<glm/common.hpp>

#define MAXKEYS 400
#define MAXMOUSEBUTTONS 10

class InputManager
{
public:
	static uint8_t keys[MAXKEYS];
	static uint8_t mousebuttons[MAXMOUSEBUTTONS];
	static double oldmouseposition[2]; //x,y
	static double newmouseposition[2]; //x,y
	static double deltamouseposition[2]; //x,y

	static void key_Callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mousebutton_Callback(GLFWwindow *window, int button, int action, int mods);
	static void mouseposition_Callback(GLFWwindow *window, double xpos, double ypos);

	static uint8_t getKeyState(uint16_t key) { return keys[key]; }
	static uint8_t getMouseButtonState(uint8_t button) { return mousebuttons[button]; }
	static glm::tvec2<double> getMouseDeltaPosition() { return glm::tvec2<double>(deltamouseposition[0],deltamouseposition[1]); };

	inline static void clearMouseDeltas() { deltamouseposition[0] = 0; deltamouseposition[1] = 0; }
};






