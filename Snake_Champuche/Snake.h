#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad\glad.h"
#include <GLFW/glfw3.h>
#include "Body.h"
class Snake
{
public:
	Snake(int bodyCount);
	void Draw(int glProgram, unsigned int VAO);
	void SetNextBody(Body* body);
	void SetPosition(float x, float y);
	void SetNextModelPosition();
	void AddBody();
	bool HitBody();
	glm::vec3 GetPosition();
	~Snake();

	int bodyCount;
	glm::mat4 model;
	Body* nextBody;
	glm::vec3 position;
	glm::vec3 lastPosition;
	Body* lastBody;
};

