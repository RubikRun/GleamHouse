#pragma once

#include "Sprite.h"
#include "Floor.h"

namespace GleamHouse
{

	class Torch;

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
		glm::vec2 getPosition() const { return m_sprite.getPositionInWorld(); }
		// Returns player's size, in world space
		glm::vec2 getSize() const;

		// Returns player's rotation, in radians
		float getRotation() const { return m_sprite.getRotation(); }
		// Sets player's rotation
		void setRotation(float rotation) { m_sprite.setRotation(rotation); }
		// Rotates player by some angle (angle is in radians)
		void rotate(float deltaRotation) { m_sprite.rotate(deltaRotation); }

		void setIsPlayable(bool isPlayable);

		// Checks if player is currently rotated so that it's facing right
		bool isFacingRight() const;

		// Checks if a given torch is close enough to player so that player can grab it
		bool canGrabTorch(const Torch& torch) const;
		// Makes player grab a given torch
		void grabTorch(Torch& torch);
		// Checks if player is currently holding a torch
		bool hasTorch() const;
		// Makes player drop the torch he's holding
		void dropTorch();
		// Checks if player is currently in a narrow tunnel.
		// A "narrow tunnel" is a horizontal/vertical tunnel that is 1 tile wide.
		bool isInNarrowTunnel(const Floor* floors, int floorsCount) const;

		const Pekan::Renderer2D::Transformable2D* getTransformable2D() const { return static_cast<const Pekan::Renderer2D::Transformable2D*>(&m_sprite); }

	private: /* functions */

		// Checks if player can be moved by some delta vector,
		// given a list of floor pieces where player is allowed to move.
		bool canMoveBy(glm::vec2 delta, const Floor* floors, int floorsCount);
		// Checks if a given point, in world space, is inside of the floor - inside one of the given floor pieces.
		bool isPointInFloor(glm::vec2 point, const Floor* floors, int floorsCount) const;

	private: /* variables */

		// Underlying sprite, used to render player's character
		Pekan::Renderer2D::Sprite m_sprite;

		// A flag indicating if player's character is currently playable,
		// meaning that it can be controlled by user.
		bool m_isPlayable = true;

		// Torch that player is currently holding
		Torch* m_torch = nullptr;
	};

} // namespace GleamHouse