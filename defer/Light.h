#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
struct LightD
{
	glm::vec3 color;
	glm::vec3 direction;

	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 m_lightMatrix;
	glm::mat4 lightMatrix;
	

	glm::mat4 getView() const
	{	
		return glm::lookAt(direction, glm::vec3(0), glm::vec3(0, 1, 0));
	}

	glm::mat4 getProjection() const
	{
		return glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	}

	void update() {}
};

struct LightP
{
	glm::vec3 color = glm::vec3(1);
	glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.f);

	struct Attenuation
	{
		float kC = 1;
		float kL = 1;
		float kQ = 1;
	};
	Attenuation attenuation;

	void update(float deltaTime)
	{
		if(position.x > maxPosition)
		{
			position.x = maxPosition;
			direction *= -1;
		}
		else if (position.x < minPosition)
		{
			position.x = minPosition;
			direction *= -1;
		}

		position.x += moveSpeed * direction * deltaTime;
	}

private:
	float maxPosition = 5;
	float minPosition = -5;
	float moveSpeed = 1;
	int direction = 1;
};