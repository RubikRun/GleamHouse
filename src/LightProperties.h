#pragma once

#include <glm/glm.hpp>

namespace GleamHouse
{

	struct LightProperties
	{
		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
		float intensity = 1.0f;
		float radius = 10.0f;
		float sharpness = 0.5f;
		bool isStar = false;
	};

} // namespace GleamHouse