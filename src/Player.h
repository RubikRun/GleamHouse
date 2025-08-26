#pragma once

#include "Sprite.h"
#include "Wall.h"

namespace GleamHouse
{

	// A class representing player's character in Gleam House.
	// Player's character
	// - is a square sprite
	// - can be moved with WASD
	// - is automatically followed by the camera
	// - is always rotated towards the mouse
	class Player
	{

	public:

		bool create();
		void destroy();

		void render() const;
		// Updates player, given a list of walls that limit player's movement.
		void update(const Wall* walls, int wallsCount);

		// Returns player's position, in world space
		glm::vec2 getPosition() const { return m_sprite.getPosition(); }

	private: /* functions */

		// Checks if player can be moved by some delta vector,
		// given a list of walls that limit player's movement.
		bool canMoveBy(glm::vec2 delta, const Wall* walls, int wallsCount);

	private: /* variables */

		// Underlying sprite, used to render player's character
		Pekan::Renderer2D::Sprite m_sprite;
	};

} // namespace GleamHouse