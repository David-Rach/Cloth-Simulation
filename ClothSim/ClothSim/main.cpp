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
#include <time.h>

/*Local Includes*/
#include "ShaderLoader.h"
#include "Cloth.h"
#include "Ball.h"
#include "Backdrop.h"

GLFWwindow* MAIN_WINDOW;
GLuint LIT_SHADER;
GLuint FLAT_SHADER;
GLuint BACKDROP_SHADER;
GLuint BALL_SHADER;
CCloth* Cloth;
CBall* Ball;
CBackdrop* Backdrop;

CCamera* Camera;
bool Fullscreen = false;
int MoveCloth = 0;

/*DeltaTime*/
double dT = 1.f/120.f;
double oldTime;
double newTime;

/*Camera things*/
float lastXpos = 600;
float lastYpos = 400;
float yaw = 270;
float pitch = 0;

void Shutdown()
{
	glfwTerminate();
	delete Cloth;
	delete Camera;
	delete Backdrop;
	delete Ball;
}

void WindowResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	Camera->resize(_width, _height);
	glViewport(0, 0, _width, _height);
}

/*Mouse callback*/
void MouseCallback(GLFWwindow* _window, double _xPos, double _yPos)
{
	/*Offset*/
	float xOffset = _xPos - lastXpos;
	float yOffset = lastYpos - _yPos;
	lastXpos = _xPos;
	lastYpos = _yPos;

	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Camera->CameraFront = glm::normalize(front);
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

	//Forward movement
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		Camera->keyArray['w'] = true;
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		Camera->keyArray['w'] = false;
	}

	//Backwards movement
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		Camera->keyArray['s'] = true;
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		Camera->keyArray['s'] = false;
	}

	//Move left
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		Camera->keyArray['a'] = true;
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		Camera->keyArray['a'] = false;
	}

	//Move right
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		Camera->keyArray['d'] = true;
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		Camera->keyArray['d'] = false;
	}

	//Move vertical
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		Camera->keyArray['e'] = true;
	}
	else if (key == GLFW_KEY_E && action == GLFW_RELEASE)
	{
		Camera->keyArray['e'] = false;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		Camera->keyArray['q'] = true;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
	{
		Camera->keyArray['q'] = false;
	}

	//boost
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		Camera->SpeedBoost = true;
	}
	else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		Camera->SpeedBoost = false;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		Camera->bMoveAlongAxis = !Camera->bMoveAlongAxis;
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		Cloth->bUseOldShading = !Cloth->bUseOldShading;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		Cloth->iDrawType = 0;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		Cloth->iDrawType = 1;
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		Cloth->iDrawType = 2;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		Cloth->bNormalMapping = !Cloth->bNormalMapping;
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
	glfwSetCursorPosCallback(MAIN_WINDOW, MouseCallback);
	glfwSetWindowSizeCallback(MAIN_WINDOW, WindowResizeCallback);

	/*Initialize the time seed*/
	srand(time(NULL));
	glPointSize(5.0f);

	return true;
}

bool OnGameplayBegin()
{
	CShaderLoader ShaderLoader;
	FLAT_SHADER = ShaderLoader.loadShaders("Assets/Shaders/flat.vs", "Assets/Shaders/flat.fs");
	LIT_SHADER = ShaderLoader.loadShaders("Assets/Shaders/lit.vs", "Assets/Shaders/lit.fs");
	BACKDROP_SHADER = ShaderLoader.loadShaders("Assets/Shaders/backdrop.vs", "Assets/Shaders/backdrop.fs");
	BALL_SHADER = ShaderLoader.loadShaders("Assets/Shaders/Ball.vs", "Assets/Shaders/Ball.fs");

	Cloth = new CCloth(64, 32); //64 particles?
	Cloth->AddTexture("Assets/Textures/Cloth/aFabric.jpg");
	Cloth->AddNormalMap("Assets/Textures/Cloth/nFabric.jpg");

	Backdrop = new CBackdrop("Assets/Textures/Backdrop.jpg");

	Ball = new CBall();
	Ball->SetClothObject(Cloth);

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

		glDisable(GL_DEPTH_TEST);
		Backdrop->Render(BACKDROP_SHADER);
		glEnable(GL_DEPTH_TEST);

		Ball->Update(1.f / 30.f);

		Cloth->Update(1.f/30.f);
		Cloth->Render(*Camera, LIT_SHADER);

		

		Camera->update(1.f / 30.0f);
		Ball->Render(*Camera, BALL_SHADER);

		float speed = 4.f;

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