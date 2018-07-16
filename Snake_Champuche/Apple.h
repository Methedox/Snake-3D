#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad\glad.h"
#include <GLFW/glfw3.h>
#include <time.h>
#include "Snake.h"
#include <iostream>

class Apple
{
public:
	Apple();
	~Apple();
	void Draw(int glProgram, unsigned int VAO);
	void SetRandomPosition(int min, int max, Snake* head);
	bool CollidesWithBody(Snake* head);
	glm::mat4 model;
	glm::vec3 position;
};

