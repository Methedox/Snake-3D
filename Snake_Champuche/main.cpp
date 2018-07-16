#include <iostream>
#include "glad\glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Snake.h"
#include "Body.h"
#include "Apple.h"
#include <vector>


std::string readFileStr(std::string path)
{
	std::ifstream fileStream;
	fileStream.open(path);

	std::stringstream strStream;
	strStream << fileStream.rdbuf();

	fileStream.close();

	return strStream.str();
}

int CreateShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	// --------------------------------------------------
	// VERTEX SHADER
	//
	std::string vertexFile = readFileStr(vertexShaderPath);
	const char* vertexShaderSource = vertexFile.c_str();

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	{
		int success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}
	// --------------------------------------------------


	// --------------------------------------------------
	// FRAGMENT SHADER
	//
	std::string fragmentFile = readFileStr(fragmentShaderPath);
	const char* fragmentShaderSource = fragmentFile.c_str();

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	{
		int success;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}
	// --------------------------------------------------

	// --------------------------------------------------
	// CREATE SHADER PROGRAM
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	{
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// --------------------------------------------------

	return shaderProgram;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Snake", NULL, NULL);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	int lightingProgram = CreateShaderProgram("lighting.vs", "lighting.fs");
	int textureProgram = CreateShaderProgram("shader.vs", "shader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	/*
	unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
	};*/

	unsigned int VAO, VBO;// , EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//// color attribute
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	// -------------- TEXTURES

	unsigned int texture;
	{
		glGenTextures(1, &texture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nbChannel;
		unsigned char *data = stbi_load("snakeskin.jpg", &width, &height, &nbChannel, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "cannot load image" << std::endl;
		}
		stbi_image_free(data);
	}
	// -----------------------

	stbi_set_flip_vertically_on_load(true);

	// -------------- TEXTURES

	unsigned int texture2;
	{
		glGenTextures(1, &texture2);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nbChannel;
		unsigned char *data = stbi_load("apple.png", &width, &height, &nbChannel, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "cannot load image" << std::endl;
		}
		stbi_image_free(data);
	}
	// -----------------------



	glViewport(0, 0, 800, 600);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);


	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -6.0f);
	glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 maxY = glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 maxX = glm::vec3(5.0f, 0.0f, 0.0f);
	glm::vec3 moveDir = glm::vec3(0.0f, 1.0f, 0.0f);

	const int ROW = 7;
	const int COL = 7;
	double lastMouseX, lastMouseY;
	lastMouseX = -1;
	lastMouseY = -1;
	bool gameOver = false;
	bool dirSet = false;
	float lastTime = 0;
	float tick = 0;
	Snake* snakeModel = new Snake(3);
	//std::vector<Body*>* bodyList = new std::vector<Body*>;
	Apple* apple = new Apple();
	apple->SetRandomPosition(-ROW, COL, snakeModel);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();


		//cameraPos.x = sin(glfwGetTime()) * 8.0f;
		//cameraPos.z = cos(glfwGetTime()) * 8.0f;
		if (!gameOver)
		{
			if (!dirSet)
			{
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				{
					if (moveDir.y != -1.0f)
					{
						moveDir.y = 1.0f;
						moveDir.x = 0.0f;
						dirSet = true;
					}
				}

				else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				{
					if (moveDir.x != -1.0f)
					{
						moveDir.x = 1.0f;
						moveDir.y = 0.0f;
						dirSet = true;
					}
				}

				else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				{
					if (moveDir.y != 1.0f)
					{
						moveDir.y = -1.0f;
						moveDir.x = 0.0f;
						dirSet = true;
					}
				}

				else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				{
					if (moveDir.x != 1.0f)
					{
						moveDir.x = -1.0f;
						moveDir.y = 0.0f;
						dirSet = true;
					}
				}
			}
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		int leftMouseBtn = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (leftMouseBtn == GLFW_PRESS)
		{
			if (lastMouseX != -1 && lastMouseY != -1)
			{
				double deltaMouseX = mouseX - lastMouseX;
				double deltaMouseY = mouseY - lastMouseY;
				cameraPos.x -= deltaMouseX * 0.005f;
				cameraPos.y += deltaMouseY * 0.005f;
			}
			lastMouseX = mouseX;
			lastMouseY = mouseY;
		}
		else
		{
			lastMouseX = -1;
			lastMouseY = -1;
		}



		targetPos = cameraPos + glm::vec3(0, 0, 1);


		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentTime = glfwGetTime();
		float delta = currentTime - lastTime;
		lastTime = currentTime;
		tick += delta;


		glUseProgram(lightingProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(lightingProgram, "texture1"), 0);

		int projectionUniformLocation = glGetUniformLocation(lightingProgram, "projection");
		glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view;
		view = glm::lookAt(cameraPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));

		int viewUniformLocation = glGetUniformLocation(lightingProgram, "view");
		glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));


		int lightPosUniformLocation = glGetUniformLocation(lightingProgram, "lightPos");
		glUniform3f(lightPosUniformLocation, 10.0f, 0.0f, -10.0f);

		int viewPosUniformLocation = glGetUniformLocation(lightingProgram, "viewPos");
		glUniform3f(viewPosUniformLocation, cameraPos.x, cameraPos.y, cameraPos.z);

		int lightColorUniformLocation = glGetUniformLocation(lightingProgram, "lightColor");
		glUniform3f(lightColorUniformLocation, 0.8f, 0.8f, 0.8f);

		if (!gameOver)
		{
			if (tick >= 0.2f)
			{
				glm::vec3 newPos = glm::vec3();
				newPos = snakeModel->GetPosition() + moveDir;
				if (newPos.x > COL)
					newPos.x = -COL;
				else if (newPos.x < -COL)
					newPos.x = COL;
				if (newPos.y > ROW)
					newPos.y = -ROW;
				else if (newPos.y < -ROW)
					newPos.y = ROW;
				snakeModel->SetPosition(newPos.x, newPos.y);
				if (snakeModel->position == apple->position)
				{
					snakeModel->AddBody();
					apple->SetRandomPosition(-ROW, COL, snakeModel);
				}
				if (snakeModel->HitBody() == true)
				{
					gameOver = true;
					std::cout << "GameOver" << std::endl;
				}
				//for (auto i = 0; i < bodyCount; i++)
				//{
				//	Body* _body = bodyList->at(i);
				//	_body->SetPosition(0.0f, 0.3f);
				//}
				dirSet = false;
				tick = 0;
			}
		}
		// -----------------------------------------------
		// ---------------- Head Model
		snakeModel->Draw(lightingProgram, VAO);
		// -----------------------------------------------


		// -----------------------------------------------
		// ---------------- TEXTURE SHADER

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUseProgram(textureProgram);
		glUniform1i(glGetUniformLocation(textureProgram, "texture1"), 1);
		//glUniform1i(glGetUniformLocation(textureProgram, "texture2"), 1);

		int transitionUniformLocation = glGetUniformLocation(textureProgram, "transition");
		glUniform1f(transitionUniformLocation, 1.0f);


		int texProjectionUniformLocation = glGetUniformLocation(textureProgram, "projection");
		glUniformMatrix4fv(texProjectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

		//glm::mat4 view;
		//view = glm::lookAt(cameraPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));

		int texViewUniformLocation = glGetUniformLocation(textureProgram, "view");
		glUniformMatrix4fv(texViewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));

		// -----------------------------------------------

		// -----------------------------------------------
		// ---------------- Apple Model
		apple->Draw(textureProgram, VAO);
		// -----------------------------------------------

		glfwSwapBuffers(window);
	}
}