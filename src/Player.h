#pragma once

#include "Sprite.h"
#include "Floor.h"

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
		// Updates player, given a list of floor pieces where player is allowed to move.
		void update(const Floor* floors, int floorsCount);

		// Returns player's position, in world space
		glm::vec2 getPosition() const { return m_sprite.getPosition(); }

	private: /* functions */

		// Checks if player can be moved by some delta vector,
		// given a list of floor pieces where player is allowed to move.
		bool canMoveBy(glm::vec2 delta, const Floor* floors, int floorsCount);

	private: /* variables */

		// Underlying sprite, used to render player's character
		Pekan::Renderer2D::Sprite m_sprite;
	};

} // namespace GleamHouse