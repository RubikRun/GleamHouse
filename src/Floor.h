#pragma once

#include "RectangleShape.h"
#include <vector>

namespace GleamHouse
{

	// A class representing the floor in Gleam House.
	// The floor is just a grid with a checkered pattern.
	class Floor
	{

	public:

		bool create();
		void destroy();

		void render() const;

	private: /* variables */

		// The way we'll render a checkered pattern
		// is NOT to render N white squares and N black squares.
		// Instead we'll render a single big white square,
		// and on top of it we'll render many small black squares.
		// So, here's a big white square, and a list of black squares:
		Pekan::Renderer2D::RectangleShape m_bigWhiteSquare;
		std::vector<Pekan::Renderer2D::RectangleShape> m_blackSquares;
	};

} // namespace GleamHouse