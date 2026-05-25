#pragma once

#include "ICamera.h"

#include <glm/vec3.hpp>
#include <cstdint>

namespace GfxEngine3D
{
	class FlyCamera : public ICamera
	{
	public:
		FlyCamera(float fov, float aspectRatio, float nearPlane, float farPlane);
		~FlyCamera() = default;

		const glm::mat4& GetViewMatrix() override;
		const glm::mat4& GetProjectionMatrix() override;

		const glm::vec3& GetPosition() const { return m_position; }
		float GetFOV() const { return m_fov; }
		float GetAspectRatio() const { return m_aspectRatio; }
		float GetNearPlane() const { return m_nearPlane; }
		float GetFarPlane() const { return m_farPlane; }
		float GetYaw() const { return m_yaw; }
		float GetPitch() const { return m_pitch; }

		void SetPosition(const glm::vec3& position);
		void SetFOV(float fov);
		void SetAspectRatio(float aspectRatio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);
		void SetYaw(float yaw);
		void SetPitch(float pitch);

		void MoveX(float delta);
		void MoveY(float delta);
		void MoveZ(float delta);

		void RotateYaw(float delta);
		void RotatePitch(float delta);

		void Zoom(float delta);

	private:
		float m_fov;
		float m_aspectRatio;
		float m_nearPlane;
		float m_farPlane;
		float m_yaw;
		float m_pitch;

		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		enum DirtyFlag
		{
			ViewMatrixFlag = 1 << 0,
			ProjectionMatrixFlag = 1 << 1,
			VectorsFlag = 1 << 2,
		};

		std::uint8_t m_dirtyFlags = ViewMatrixFlag | ProjectionMatrixFlag | VectorsFlag;

		void UpdateVectorsIfNeeded();
		void UpdateViewMatrixIfNeeded();
		void UpdateProjectionMatrixIfNeeded();

		glm::mat4 CalculateViewMatrix() const;
		glm::mat4 CalculateProjectionMatrix() const;
		glm::vec3 CalculateFrontVector() const;
		glm::vec3 CalculateRightVector() const;

		static inline float Clamp(float value, float min, float max)
		{
			if (value < min)
				return min;
			if (value > max)
				return max;
			return value;
		}
	};
}
