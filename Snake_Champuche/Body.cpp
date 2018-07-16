#include "Body.h"

Body::Body()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

}

void Body::Draw(int glProgram, unsigned int VAO)
{
	int modelLoc = glGetUniformLocation(glProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	if (nextBody != nullptr)
		nextBody->Draw(glProgram, VAO);
}

void Body::SetNextBody(Body* body)
{
		nextBody = body;
}

void Body::SetPosition(float x, float y)
{
	lastPosition = position;
	position = glm::vec3(x, y, 0.0f);
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::translate(model, position);
	SetNextModelPosition();
}

void Body::SetNextModelPosition()
{
	if (nextBody != nullptr)
		nextBody->SetPosition(lastPosition.x, lastPosition.y);
}

glm::vec3 Body::GetPosition()
{
	return position;
}

Body::~Body()
{
}
