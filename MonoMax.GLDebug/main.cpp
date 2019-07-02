// MonoMax.GLDebug.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../MonoMax.EngineCore/Core.h"
#include <iostream>

MonoMaxEngine::GraphicsEngine* engine;

int curX, curY;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	engine->Resize(width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	int btnCode = - 1;
	int btnState = -1;

	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT: btnCode = 1; break;
		case GLFW_MOUSE_BUTTON_RIGHT: btnCode = 2; break;
		case GLFW_MOUSE_BUTTON_MIDDLE: btnCode = 3; break;
	}

	if (action == GLFW_PRESS)
		btnState = 1;
	else if(action == GLFW_RELEASE)
		btnState = 0;

	//std::cout << "btn=" << btnCode << "mode=" << btnMode << std::endl;


	if (btnState == 1)
		engine->MouseDown(curX, curY, btnCode);
	else if (btnState == 0)
		engine->MouseUp();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	int delta = 0;

	if (yoffset < 0.0)
		delta = -1;
	else
		delta = 1;

	engine->MouseScroll(delta);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	curX = (int)xpos;
	curY = (int)ypos;

	engine->MouseMove(curX, curY);

	//std::cout << "X=" << xpos << " Y=" << ypos << "\n";
}

int main()
{
	engine = new MonoMaxEngine::GraphicsEngine();
	engine->Init(false);
	GLFWwindow* window = engine->GetWindow();
	
	// callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	engine->Resize(640, 480);
	engine->AddMesh("C:/tmp/stl/test_1.stl");

	while (!glfwWindowShouldClose(window))
	{
		engine->Mainloop(nullptr);
	}

	engine->Terminate();
	delete engine;

}
