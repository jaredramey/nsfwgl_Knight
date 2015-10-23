#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "../nsfwgl/nsfw.h"

struct Camera
{
	bool StartupPerspective(const float fov, const float aspectRatio, const float a_near, const float a_far);
	bool SetView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up);

	glm::mat4 transform;
	float Near, Far, aspect, fov;

	Camera() : Near(1), Far(100), aspect(800 / 600.f), fov(90) {}

	void update() {}
	void lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up) { transform = glm::inverse(glm::lookAt(pos, target, up)); }
	glm::mat4 getView()		  const { return glm::inverse(transform); }
	glm::mat4 getProjection() const { return glm::perspective(fov, aspect, Near, Far); }

protected:
	glm::vec3 mPosition, mTarget, mUpVector;
	float mLeft, mRight, mBottom, mTop;

	glm::mat4 mWorldTransform = glm::mat4();

	glm::mat4 mViewTransform = glm::mat4();//draw transform
	glm::mat4 mProjectionTransform = glm::mat4();
	glm::mat4 mProjectionViewTransform = glm::mat4();

	void UpdateProjectViewTransform();
	void UpdateView();
};