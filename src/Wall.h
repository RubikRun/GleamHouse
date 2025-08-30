#pragma once

#include "Sprite.h"
#include "RectangleShape.h"
#include "BoundingBox.h"

namespace GleamHouse
{

	// A class representing a wall in Gleam House
	class Wall
	{

	public:

		bool create(glm::vec2 bottomLeftPosition, glm::vec2 topRightPosition);
		void destroy();

		void render() const;

	private: /* variables */

		// Underlying sprite, used to render wall
		Pekan::Renderer2D::Sprite m_sprite;
	};

} // namespace GleamHouse