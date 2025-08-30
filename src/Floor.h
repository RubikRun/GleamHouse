#pragma once

#include "RectangleShape.h"
#include "BoundingBox.h"
#include <vector>

namespace GleamHouse
{

	// A class representing a piece of floor in Gleam House
	class Floor
	{

	public:

		bool create(glm::ivec2 bottomLeftPosition, glm::ivec2 topRightPosition);
		void destroy();

		void render() const;

		// Returns floor's bounding box, determining how the floor will collide with other objects
		BoundingBox getBoundingBox() const { return m_boundingBox; }

	private: /* variables */

		// The way we'll render a checkered pattern
		// is NOT to render ~N white squares and ~N black squares.
		// Instead we'll render a single big white rectangle,
		// and on top of it we'll render many small black squares.
		// So, here's a big white rectangle, and a list of black squares:
		Pekan::Renderer2D::RectangleShape m_bigWhiteRectangle;
		std::vector<Pekan::Renderer2D::RectangleShape> m_blackSquares;

		// Floor's bounding box, determining how the floor will collide with other objects
		BoundingBox m_boundingBox;
	};

} // namespace GleamHouse