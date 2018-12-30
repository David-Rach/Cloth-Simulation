/*
   _____ _      ____ _______ _    _    _____ _____ __  __
  / ____| |    / __ |__   __| |  | |  / ____|_   _|  \/  |
 | |    | |   | |  | | | |  | |__| | | (___   | | | \  / |
 | |    | |   | |  | | | |  |  __  |  \___ \  | | | |\/| |
 | |____| |___| |__| | | |  | |  | |  ____) |_| |_| |  | |
  \_____|______\____/  |_|  |_|  |_| |_____/|_____|_|  |_|

  A cloth simulation by David Rach
  December 30th 2018
*/

#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include "ShaderLoader.h"

GLFWwindow* MAIN_WINDOW;
GLuint LIT_SHADER;

bool Init()
{
	/*Initialize GLFW library*/
	if (!glfwInit())
	{
		//returns a nope to the mainframe
		std::cout << "ERROR!!" << std::endl;
		std::cout << "Aww. That's a nope on the GLFW initialize. Eheh... Get help." << std::endl;
		system("pause");
		glfwTerminate();
		return false;
	}

	/*Create a window*/
	glfwWindowHint(GLFW_SAMPLES, 4);
	MAIN_WINDOW = glfwCreateWindow(1200, 800, "Cloth Simulation", NULL, NULL);
	if (!MAIN_WINDOW)
	{
		//If the window fails to be created, there is quite literally no point in continuing
		std::cout << "ERROR!!" << std::endl;
		std::cout << "Window failed to open. Did you open the curtain?" << std::endl;
		system("pause");
		glfwTerminate();
		return false;
	}

	/*make this window the current context*/
	glfwMakeContextCurrent(MAIN_WINDOW);

	/*initailize GLEW library*/
	GLenum glewChk = glewInit();
	if (GLEW_OK != glewChk)
	{
		std::cout << "Failed to initiate GLEW library. Something is seriously wrong" << std::endl;
		std::cout << stderr, "Error: %s\n", glewGetErrorString(GLEW_VERSION);
		system("pause");
		glfwTerminate();
		return false;
	}

	/*Set callbacks*/
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(MAIN_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

bool OnGameplayBegin()
{
	CShaderLoader ShaderLoader;
	LIT_SHADER = ShaderLoader.loadShaders("Assets/Shaders/lit.vs", "Assets/Shaders/lit.fs");

	return true;
}

int main()
{
	if (Init())
	{
		std::cout << "Initiation Successful!" << std::endl;
		std::cout << "Beginning shader loadup..." << std::endl;
	}

	if (OnGameplayBegin())
	{
		std::cout << "Gameplay started!" << std::endl;
	}

	while (!glfwWindowShouldClose(MAIN_WINDOW))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);



		glfwSwapBuffers(MAIN_WINDOW);
		glfwPollEvents();
	}
}