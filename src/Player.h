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
		// Returns player's size, in world space
		glm::vec2 getSize() const;

		// Sets player's rotation
		void setRotation(float rotation) { m_sprite.setRotation(rotation); }
		// Rotates player by some angle (angle is in radians)
		void rotate(float deltaRotation) { m_sprite.rotate(deltaRotation); }

		void setIsPlayable(bool isPlayable);

		// Checks if player is currently rotated so that it's facing right
		bool isFacingRight() const;

	private: /* functions */

		// Checks if player can be moved by some delta vector,
		// given a list of floor pieces where player is allowed to move.
		bool canMoveBy(glm::vec2 delta, const Floor* floors, int floorsCount);

	private: /* variables */

		// Underlying sprite, used to render player's character
		Pekan::Renderer2D::Sprite m_sprite;

		// A flag indicating if player's character is currently playable,
		// meaning that it can be controlled by user.
		bool m_isPlayable = true;
	};

} // namespace GleamHouse