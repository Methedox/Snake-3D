#include "Snake.h"



Snake::Snake(int bodyCount)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	bodyCount = 2;

	for (size_t i = 0; i < bodyCount; i++)
	{
		Body* bodyModel = new Body();
		bodyModel->SetPosition(0.0f, -1.0f*(i + 1));
		//bodyList->push_back(bodyModel);
		if (i == 0)
			this->SetNextBody(bodyModel);
		else
			lastBody->SetNextBody(bodyModel);
		lastBody = bodyModel;
	}
}

void Snake::Draw(int glProgram, unsigned int VAO)
{
	int modelLoc = glGetUniformLocation(glProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	if (nextBody != nullptr)
		nextBody->Draw(glProgram, VAO);
}

void Snake::SetNextBody(Body* body)
{
		nextBody = body;
}

void Snake::SetPosition(float x, float y)
{
	lastPosition = position;
	position = glm::vec3(x, y, 0.0f);
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::translate(model, position);
	SetNextModelPosition();
}

void Snake::SetNextModelPosition()
{
	if (nextBody != nullptr)
		nextBody->SetPosition(lastPosition.x, lastPosition.y);
}

void Snake::AddBody()
{
	Body* newBody = new Body();
	newBody->SetPosition(lastBody->position.x, lastBody->position.y);
	lastBody->SetNextBody(newBody);
	lastBody = newBody;
}

bool Snake::HitBody()
{
	Body* curBody;
	curBody = this->nextBody;
	while (curBody->nextBody != nullptr)
	{
		if (this->position == curBody->lastPosition)
			return true;
		else
			curBody = curBody->nextBody;
	}
	return false;
}

glm::vec3 Snake::GetPosition()
{
	return position;
}

Snake::~Snake()
{
}
