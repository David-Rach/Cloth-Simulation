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

/*External Includes*/
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vld.h>

/*Local Includes*/
#include "ShaderLoader.h"
#include "Cloth.h"

GLFWwindow* MAIN_WINDOW;
GLuint LIT_SHADER;
GLuint FLAT_SHADER;
CCloth* Cloth;
CCamera* Camera;
bool Fullscreen = false;
int MoveCloth = 0;

/*DeltaTime*/
double dT;
double oldTime;
double newTime;

void Shutdown()
{
	glfwTerminate();
	delete Cloth;
	delete Camera;
}

void WindowResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	Camera->resize(_width, _height);
	glViewport(0, 0, _width, _height);
}

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(MAIN_WINDOW,true);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		MoveCloth = 1;
	}
	else if (key == GLFW_KEY_P && action == GLFW_RELEASE)
	{
		MoveCloth = 0;
	}

	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		MoveCloth = 2;
	}
	else if (key == GLFW_KEY_O && action == GLFW_RELEASE)
	{
		MoveCloth = 0;
	}

	//Fullscreen
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (!Fullscreen)
		{
			float width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
			float height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
			Fullscreen = true;

			glfwSetWindowMonitor(MAIN_WINDOW, glfwGetWindowMonitor(MAIN_WINDOW), 0, 0, width, height, 60);
		}
		else
		{
			glfwSetWindowMonitor(MAIN_WINDOW, NULL, 100, 100, 1200, 800, 60);
			Fullscreen = false;
		}
	}
}

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
	glfwSetKeyCallback(MAIN_WINDOW, KeyboardCallback);
	glfwSetWindowSizeCallback(MAIN_WINDOW, WindowResizeCallback);

	return true;
}

bool OnGameplayBegin()
{
	CShaderLoader ShaderLoader;
	FLAT_SHADER = ShaderLoader.loadShaders("Assets/Shaders/flat.vs", "Assets/Shaders/flat.fs");
	LIT_SHADER = ShaderLoader.loadShaders("Assets/Shaders/lit.vs", "Assets/Shaders/lit.fs");

	Cloth = new CCloth(128, 64); //64 particles?
	Camera = new CCamera(1200, 800);
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

		/*Lets calculate deltaTime*/
		oldTime = newTime;
		newTime = glfwGetTime();

		dT = newTime - oldTime;

		Cloth->Update(1.f/120.f);
		Cloth->Render(*Camera, LIT_SHADER);

		float speed = 20.f;

		if (MoveCloth == 1)
		{
			CParticle* Particle1 = Cloth->GetParticle(Cloth->GetIndexFromGridCoord(0, Cloth->m_height - 1));
			Particle1->SetPosition(Particle1->GetPosition() + (glm::vec3(1, 0, 1) * (float)dT) * speed);

			CParticle* Particle2 = Cloth->GetParticle(Cloth->GetIndexFromGridCoord(Cloth->m_width - 1, Cloth->m_height - 1));
			Particle2->SetPosition(Particle2->GetPosition() + (glm::vec3(-1, 0, 1) * (float)dT) * speed);
		}
		else if (MoveCloth == 2)
		{
			CParticle* Particle1 = Cloth->GetParticle(Cloth->GetIndexFromGridCoord(0, Cloth->m_height - 1));
			Particle1->SetPosition(Particle1->GetPosition() + (glm::vec3(-1, 0, -1) * (float)dT) * speed);

			CParticle* Particle2 = Cloth->GetParticle(Cloth->GetIndexFromGridCoord(Cloth->m_width - 1, Cloth->m_height - 1));
			Particle2->SetPosition(Particle2->GetPosition() + (glm::vec3(1, 0, -1) * (float)dT) * speed);
		}

		glfwSwapBuffers(MAIN_WINDOW);
		glfwPollEvents();
	}

	Shutdown();
}