#pragma once

#include "Sprite.h"

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
		void update();

	private: /* variables */

		// Underlying sprite, used to render player's character
		Pekan::Renderer2D::Sprite m_sprite;
	};

} // namespace GleamHouse