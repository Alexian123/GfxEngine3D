#include "FlyCamera.h"

#include <glm/gtc/matrix_transform.hpp>

FlyCamera::FlyCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
	: m_fov(fov), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane)
{
	m_yaw = -90.0f;	// looking towards negative z-axis
	m_pitch = 0.0f;
	m_position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	UpdateVectorsIfNeeded();
	UpdateViewMatrixIfNeeded();
	UpdateProjectionMatrixIfNeeded();
}

const glm::mat4& FlyCamera::GetViewMatrix()
{
	UpdateVectorsIfNeeded();
	UpdateViewMatrixIfNeeded();
	return m_viewMatrix;
}

const glm::mat4& FlyCamera::GetProjectionMatrix()
{
	UpdateProjectionMatrixIfNeeded();
	return m_projectionMatrix;
}

void FlyCamera::SetPosition(const glm::vec3& position)
{
	m_position = position;
	m_dirtyFlags |= ViewMatrixFlag;
}

void FlyCamera::SetFOV(float fov)
{
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
	m_fov = fov;
	m_dirtyFlags |= ProjectionMatrixFlag;
}

void FlyCamera::SetAspectRatio(float aspectRatio)
{
	m_aspectRatio = aspectRatio;
	m_dirtyFlags |= ProjectionMatrixFlag;
}

void FlyCamera::SetNearPlane(float nearPlane)
{
	m_nearPlane = nearPlane;
	m_dirtyFlags |= ProjectionMatrixFlag;
}

void FlyCamera::SetFarPlane(float farPlane)
{
	m_farPlane = farPlane;
	m_dirtyFlags |= ProjectionMatrixFlag;
}

void FlyCamera::SetYaw(float yaw)
{
	m_yaw = yaw;
	m_dirtyFlags |= VectorsFlag | ViewMatrixFlag;
}

void FlyCamera::SetPitch(float pitch)
{
	m_pitch = Clamp(pitch, -89.0f, 89.0f);
	m_dirtyFlags |= VectorsFlag | ViewMatrixFlag;
}

void FlyCamera::MoveX(float delta)
{
	UpdateVectorsIfNeeded();
	m_position += m_right * delta;
	m_dirtyFlags |= ViewMatrixFlag;
}

void FlyCamera::MoveY(float delta)
{
	m_position += m_up * delta;
	m_dirtyFlags |= ViewMatrixFlag;
}

void FlyCamera::MoveZ(float delta)
{
	UpdateVectorsIfNeeded();
	m_position += m_front * delta;
	m_dirtyFlags |= ViewMatrixFlag;
}

void FlyCamera::RotateYaw(float delta)
{
	m_yaw += delta;
	m_dirtyFlags |= VectorsFlag | ViewMatrixFlag;
}

void FlyCamera::RotatePitch(float delta)
{
	m_pitch += delta;
	m_pitch = Clamp(m_pitch, -89.0f, 89.0f);
	m_dirtyFlags |= VectorsFlag | ViewMatrixFlag;
}

void FlyCamera::Zoom(float delta)
{
	SetFOV(m_fov + delta);
}

void FlyCamera::UpdateVectorsIfNeeded()
{
	if ((m_dirtyFlags & VectorsFlag) != 0) {
		m_front = CalculateFrontVector();
		m_right = CalculateRightVector();
		m_dirtyFlags &= ~VectorsFlag;
		// Mark view matrix dirty since it depends on the vectors
		m_dirtyFlags |= ViewMatrixFlag;
	}
}

void FlyCamera::UpdateViewMatrixIfNeeded()
{
	if ((m_dirtyFlags & ViewMatrixFlag) != 0) {
		m_viewMatrix = CalculateViewMatrix();
		m_dirtyFlags &= ~ViewMatrixFlag;
	}
}

void FlyCamera::UpdateProjectionMatrixIfNeeded()
{
	if ((m_dirtyFlags & ProjectionMatrixFlag) != 0) {
		m_projectionMatrix = CalculateProjectionMatrix();
		m_dirtyFlags &= ~ProjectionMatrixFlag;
	}
}

glm::mat4 FlyCamera::CalculateViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 FlyCamera::CalculateProjectionMatrix() const
{
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}

glm::vec3 FlyCamera::CalculateFrontVector() const
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	return glm::normalize(direction);
}

glm::vec3 FlyCamera::CalculateRightVector() const
{
	return glm::normalize(glm::cross(m_front, m_up));
}