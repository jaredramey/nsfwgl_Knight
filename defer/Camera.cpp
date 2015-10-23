#include "Camera.h"

bool Camera::StartupPerspective(const float fov, const float aspectRatio, const float near, const float far)
{
	mProjectionTransform = glm::perspective(fov, aspectRatio, near, far);
	UpdateProjectViewTransform();
	return true;
}

bool Camera::SetView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	mPosition = position;
	mTarget = target;
	mUpVector = up;

	UpdateView();

	return true;
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