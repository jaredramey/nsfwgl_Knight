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
	glm::vec3 color;
	glm::vec3 position;
	float attenuation;
};