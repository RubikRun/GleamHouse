#pragma once

#include "Sprite.h"
#include "RectangleShape.h"

namespace GleamHouse
{

	// A class representing a wall in Gleam House.
	// A wall
	// - is a sprite
	// - doesn't allow player to pass through it
	class Wall
	{

	public:

		bool create(glm::vec2 bottomLeftPosition, glm::vec2 topRightPosition);
		void destroy();

		void render() const;

	private: /* variables */

		// Underlying sprite, used to render wall
		Pekan::Renderer2D::Sprite m_sprite;
		// Underlying rectangle, used to render wall's border
		Pekan::Renderer2D::RectangleShape m_rectangle;
	};

} // namespace GleamHouse