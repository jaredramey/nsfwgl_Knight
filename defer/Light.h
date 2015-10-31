#pragma once
#include <glm/glm.hpp>

struct LightD
{
	glm::vec3 color;
	glm::vec3 direction;

	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 m_lightMatrix;
	glm::mat4 lightMatrix;

	void update() {}
};

struct LightP
{
	glm::vec3 color;
	glm::vec3 position;
	float attenuation;
};