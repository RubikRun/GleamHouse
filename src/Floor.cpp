#include "Floor.h"

#include "PekanLogger.h"

using namespace Pekan::Renderer2D;

namespace GleamHouse
{

	// Size of a single tile, in world space
	static const float FLOOR_TILE_SIZE = 1.0f;
	// Number of tiles to be rendered
	//
	// NOTE: This number has to be even!
	static const int FLOOR_TILES_COUNT = 100;

	// Exact color of "white" tiles
	static const glm::vec4 COLOR_WHITE = { 0.7f, 0.7f, 0.7f, 1.0f };
	// Exact color of "black" tiles
	static const glm::vec4 COLOR_BLACK = { 0.1f, 0.1f, 0.1f, 1.0f };

	bool Floor::create()
	{
		// Create big white square
		const float bigWhiteSquareSize = FLOOR_TILE_SIZE * float(FLOOR_TILES_COUNT);
		m_bigWhiteSquare.create(bigWhiteSquareSize, bigWhiteSquareSize);
		m_bigWhiteSquare.setColor(COLOR_WHITE);

		// Create black squares
		{
			PK_ASSERT_QUICK(m_blackSquares.empty());
			// Allocate memory for all black squares
			m_blackSquares.resize(FLOOR_TILES_COUNT * FLOOR_TILES_COUNT / 2);
			// Number of black squares created so far
			int blackSquaresCreatedCount = 0;
			for (int row = 0; row < FLOOR_TILES_COUNT; row++)
			{
				// Rows alternate between starting with a black square and starting with a white square,
				// so the starting column of the black squares in each row is 0, 1, 0, 1, 0, etc.
				const int startingCol = (row % 2 == 0) ? 0 : 1;
				for (int col = startingCol; col < FLOOR_TILES_COUNT; col += 2)
				{
					// Calculate coordinates of square's center, in world space
					const float xCenter = (col - FLOOR_TILES_COUNT / 2) * FLOOR_TILE_SIZE + FLOOR_TILE_SIZE / 2.0f;
					const float yCenter = (row - FLOOR_TILES_COUNT / 2) * FLOOR_TILE_SIZE + FLOOR_TILE_SIZE / 2.0f;
					// Create a black square there
					RectangleShape& blackSquare = m_blackSquares[blackSquaresCreatedCount++];
					blackSquare.create(FLOOR_TILE_SIZE, FLOOR_TILE_SIZE);
					blackSquare.setPosition({ xCenter, yCenter });
					blackSquare.setColor(COLOR_BLACK);
				}
			}
		}

		return true;
	}

	void Floor::destroy()
	{
		for (RectangleShape& blackSquare : m_blackSquares)
		{
			blackSquare.destroy();
		}
		m_bigWhiteSquare.destroy();
	}

	void Floor::render() const
	{
		m_bigWhiteSquare.render();
		for (const RectangleShape& blackSquare : m_blackSquares)
		{
			blackSquare.render();
		}
	}

} // namespace GleamHouse
