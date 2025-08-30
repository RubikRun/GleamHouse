#include "Floor.h"

#include "PekanLogger.h"

using namespace Pekan::Renderer2D;

namespace GleamHouse
{

	// Exact color of "white" tiles
	static constexpr glm::vec4 COLOR_WHITE = { 0.7f, 0.7f, 0.7f, 1.0f };
	// Exact color of "black" tiles
	static constexpr glm::vec4 COLOR_BLACK = { 0.1f, 0.1f, 0.1f, 1.0f };

	bool Floor::create(glm::ivec2 bottomLeftPosition, glm::ivec2 topRightPosition)
	{
		// Create big white rectangle
		const glm::vec2 bigWhiteRectangleSize = glm::vec2(topRightPosition - bottomLeftPosition);
		PK_ASSERT_QUICK(bigWhiteRectangleSize.x > 0.0f && bigWhiteRectangleSize.y > 0.0f);
		m_bigWhiteRectangle.create(bigWhiteRectangleSize.x, bigWhiteRectangleSize.y);
		m_bigWhiteRectangle.setPosition(glm::vec2(bottomLeftPosition) + bigWhiteRectangleSize / 2.0f);
		m_bigWhiteRectangle.setColor(COLOR_WHITE);

		// Create black squares
		{
			PK_ASSERT_QUICK(m_blackSquares.empty());
			const glm::ivec2 squaresCount = topRightPosition - bottomLeftPosition;
			// Allocate memory for all black squares.
			// (*) See formula derivation at the end of the file
			m_blackSquares.resize
			(
				((squaresCount.x + 1) / 2) * ((squaresCount.y + 1) / 2) + (squaresCount.x / 2) * (squaresCount.y / 2)
			);
			// Number of black squares created so far
			int blackSquaresCreatedCount = 0;
			for (int row = 0; row < squaresCount.y; row++)
			{
				// Rows alternate between starting with a black square and starting with a white square,
				// so the starting column of the black squares in each row is 0, 1, 0, 1, 0, etc.
				const int startingCol = (row % 2 == 0) ? 0 : 1;
				for (int col = startingCol; col < squaresCount.x; col += 2)
				{
					// Calculate coordinates of square's center, in world space
					const float xCenter = float(col + bottomLeftPosition.x) + 0.5f;
					const float yCenter = float(row + bottomLeftPosition.y) + 0.5f;
					// Create a black square there
					RectangleShape& blackSquare = m_blackSquares[blackSquaresCreatedCount++];
					blackSquare.create(1.0f, 1.0f);
					blackSquare.setPosition({ xCenter, yCenter });
					blackSquare.setColor(COLOR_BLACK);
				}
			}
			PK_ASSERT_QUICK(blackSquaresCreatedCount == m_blackSquares.size());
		}

		// Create bounding box
		m_boundingBox.min = glm::vec2(bottomLeftPosition);
		m_boundingBox.max = glm::vec2(topRightPosition);

		return true;
	}

	void Floor::destroy()
	{
		for (RectangleShape& blackSquare : m_blackSquares)
		{
			blackSquare.destroy();
		}
		m_bigWhiteRectangle.destroy();
	}

	void Floor::render() const
	{
		m_bigWhiteRectangle.render();
		for (const RectangleShape& blackSquare : m_blackSquares)
		{
			blackSquare.render();
		}
	}

} // namespace GleamHouse




// (*)
// Formula derivation:
//     Let X be the number of squares horizontally and Y be the number of squares vertically.
//     We want to know the number of black squares.
//     We'll assume that the row 0 starts with a black square, so the board looks like:
//         BWBWBW...
//         WBWBWB...
//         BWBWBW...
//         WBWBWB...
//         ...
//    Then, row 0, row 2, row 4, etc. will have (X + 1) / 2 black squares each,
//    and there are (Y + 1) / 2 of these rows, so the total number of black squares from these rows is:
//        ((X + 1) / 2) * ((Y + 1) / 2)
//    Similarly, row 1, row 3, row 5, etc. will have X / 2 black squares each,
//    and there are Y / 2 of these rows, so the total number of black squares from these rows is:
//        (X / 2) * (Y / 2)
//    Finally, adding these 2 quantities together we get the total number of black squares on the whole board:
//        ((X + 1) / 2) * ((Y + 1) / 2) + (X / 2) * (Y / 2)
//    IMPORTANTLY: All these divisions by 2 are WHOLE NUMBER DIVISIONS
//                 so do NOT try to simplify the formula with standard maths.
