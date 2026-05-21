#pragma once

#include <glm/mat4x4.hpp>

class ICamera
{
public:
	virtual ~ICamera() = default;
	virtual const glm::mat4& GetViewMatrix() = 0;
	virtual const glm::mat4& GetProjectionMatrix() = 0;
};

