#include "Camera.h"

void Camera::Slide(const float hDistance, const float vDistance)
{
	mPosition += glm::vec3(hDistance, vDistance, 0);
	mTarget += glm::vec3(hDistance, vDistance, 0);
	UpdateView();
}

void Camera::Move(const float distance)
{
	glm::vec3 direction = glm::normalize(mTarget - mPosition);
	mPosition += distance * direction;
	mTarget += distance * direction;
	UpdateView();
}

bool Camera::StartupPerspective(const float fov, const float aspectRatio, const float near, const float far)
{
	mProjectionTransform = glm::perspective(fov, aspectRatio, near, far);
	UpdateProjectViewTransform();
	return true;
}

void Camera::Update(const float deltaTime)
{
}

const glm::mat4 Camera::GetWorldTransform()
{
	return glm::inverse(mViewTransform);
}

glm::mat4 Camera::GetView()
{
	return mViewTransform;
}

glm::mat4 Camera::GetProjection()
{
	return mProjectionTransform;
}

glm::mat4 Camera::GetViewProjection()
{
	return mProjectionViewTransform;
}

void Camera::UpdateProjectViewTransform()
{
	mProjectionViewTransform = mProjectionTransform * mViewTransform;
}

void Camera::UpdateView()
{
	mViewTransform = glm::lookAt(mPosition, mTarget, mUpVector);
	mWorldTransform = glm::inverse(mViewTransform);
	UpdateProjectViewTransform();
}

bool Camera::SetView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	mPosition = position;
	mTarget = target;
	mUpVector = up;

	UpdateView();

	return true;
}

