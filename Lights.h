#pragma once

#include <glm/vec3.hpp>

namespace GfxEngine3D
{
    struct LightColor
    {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct DirLight
    {
        LightColor color;
        glm::vec3 direction;
    };

    struct PointLight
    {
        LightColor color;
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
    };

    struct SpotLight
    {
        PointLight pl;
        glm::vec3 direction;
        float cutOff;
        float outerCutOff;
    };
}