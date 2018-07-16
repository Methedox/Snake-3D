#include "Apple.h"



Apple::Apple()
{
	srand(time(NULL));
}


Apple::~Apple()
{
}



void Apple::Draw(int glProgram, unsigned int VAO)
{
	int modelLoc = glGetUniformLocation(glProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//std::cout << position.x << " - " << position.y << std::endl;
}

void Apple::SetRandomPosition(int min, int max, Snake* snake)
{
	position.x = (int)rand() % (max - min) + min;
	position.y = (int)rand() % (max - min) + min;
	position.z = 0;

	while (CollidesWithBody(snake) == true)
	{
		position.x = (int)rand() % (max - min) + min;
		position.y = (int)rand() % (max - min) + min;
		position.z = 0;
	}
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::translate(model, position);
}

bool Apple::CollidesWithBody(Snake * snake)
{
	Body* curBody;
	curBody = snake->nextBody;
	while (curBody->nextBody != nullptr)
	{
		if (this->position == curBody->lastPosition)
			return true;
		else
			curBody = curBody->nextBody;
	}
	return false;
}
