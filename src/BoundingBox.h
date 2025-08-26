#pragma once

#include <glm/glm.hpp>

namespace GleamHouse
{

	struct BoundingCircle;

	// A class representing an axis-alligned bounding box,
	// which is a rectangle in world space,
	// usually surrounding an object
	// and used for detecting collisions between that object and other objects.
	struct BoundingBox
	{
		glm::vec2 min = { 0.0f, 0.0f };
		glm::vec2 max = { 0.0f, 0.0f };

		// Checks if a bounding box collides with another bounding box
		bool collides(BoundingBox other) const;
		// Checks if a bounding box collides with a bounding circle
		bool collides(const BoundingCircle& circle) const;
	};

} // namespace GleamHouse