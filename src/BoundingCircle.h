#pragma once

#include <glm/glm.hpp>

namespace GleamHouse
{

	struct BoundingBox;

	// A class representing a bounding circle,
	// which is a circle in world space,
	// usually surrounding an object
	// and used for detecting collisions between that object and other objects.
	struct BoundingCircle
	{
		glm::vec2 position = { 0.0f, 0.0f };
		float radius = 0.0f;

		// Checks if a bounding circle collides with another bounding circle
		bool collides(BoundingCircle other) const;
		// Checks if a bounding circle collides with a bounding box
		bool collides(const BoundingBox& box) const;
	};

} // namespace GleamHouse