#include "Torch.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"

using namespace Pekan::Renderer2D;
using namespace Pekan::Utils;

namespace GleamHouse
{

	// Color of torch's base
	static constexpr glm::vec4 COLOR_BASE = { 0.369f, 0.204f, 0.059f, 1.0f };
	// Size of torch's base, in world space
	static constexpr glm::vec2 SIZE_BASE = { 0.15f, 0.6f };
	// Size of torch's fire, in world space
	static constexpr glm::vec2 SIZE_FIRE = { SIZE_BASE.x * 1.5f, SIZE_BASE.y * 1.2f };
	// Total number of lines that torch's fire is made up of
	static constexpr int COUNT_FIRE_LINES = 20;
	// Thickness of a line of fire
	static constexpr float THICKNESS_FIRE_LINE = 0.05f;
	// Time between fire colors updates, in seconds
	static constexpr float TIME_BETWEEN_FIRE_COLORS_UPDATES = 0.1f;

	// Returns a random fire-ish color that can be used for a single line of the fire
	static glm::vec4 getRandomFireColor()
	{
		static const glm::vec4 fireColorsPalette[10] = {
			{1.0f, 0.25f, 0.0f, 1.0f},   // deep orange-red
			{1.0f, 0.35f, 0.0f, 1.0f},   // orange-red
			{1.0f, 0.45f, 0.0f, 1.0f},   // bright orange
			{1.0f, 0.55f, 0.0f, 1.0f},   // orange with yellow tint
			{1.0f, 0.65f, 0.0f, 1.0f},   // lighter orange
			{1.0f, 0.75f, 0.1f, 1.0f},   // orange-yellow
			{1.0f, 0.85f, 0.2f, 1.0f},   // warm yellow-orange
			{1.0f, 0.95f, 0.3f, 1.0f},   // golden yellow
			{1.0f, 1.0f, 0.4f, 1.0f},    // bright yellow
			{1.0f, 0.9f, 0.2f, 1.0f}     // fiery yellow-orange
		};
		const int index = getRandomInt(0, 9);
		return fireColorsPalette[index];
	}

	bool Torch::create(glm::vec2 position)
	{
		m_position = position;

		// Create base
		m_base.create(SIZE_BASE.x, SIZE_BASE.y);
		m_base.setPosition(position);
		m_base.setColor(COLOR_BASE);

		// Create fire
		{
			PK_ASSERT_QUICK(m_fire.empty());
			m_fire.resize(COUNT_FIRE_LINES);

			// Offset between two consecutive lines in the middle of the fire
			const float lineOffset = SIZE_FIRE.x / float(COUNT_FIRE_LINES / 2 - 1);
			// Point where the bottom half of fire lines join together
			const glm::vec2 bottomJoinPoint = { position.x, position.y + SIZE_BASE.y / 2.0f };
			// Point where the top half of fire lines join together
			const glm::vec2 topJoinPoint = { bottomJoinPoint.x, bottomJoinPoint.y + SIZE_FIRE.y };
			// Y coordinate of fire's middle
			const float yMiddle = bottomJoinPoint.y + SIZE_FIRE.y / 2.0f;
			// X coordinate of fire's leftmost point
			const float xLeft = position.x - SIZE_FIRE.x / 2.0f;

			// Create bottom half of fire
			for (int i = 0; i < COUNT_FIRE_LINES / 2; i++)
			{
				m_fire[i].create
				(
					bottomJoinPoint,
					{ xLeft + lineOffset * float(i), yMiddle + SIZE_FIRE.y / 100.0f }
				);
				m_fire[i].setColor(getRandomFireColor());
				m_fire[i].setThickness(THICKNESS_FIRE_LINE);
			}
			// Create top half of fire
			for (int i = 0; i < COUNT_FIRE_LINES / 2; i++)
			{
				m_fire[i + COUNT_FIRE_LINES / 2].create
				(
					topJoinPoint,
					{ xLeft + lineOffset * float(i), yMiddle - SIZE_FIRE.y / 100.0f }
				);
				m_fire[i + COUNT_FIRE_LINES / 2].setColor(getRandomFireColor());
				m_fire[i + COUNT_FIRE_LINES / 2].setThickness(THICKNESS_FIRE_LINE);
			}
		}

		// Create bounding box
		m_boundingBox.min = position - SIZE_BASE / 2.0f;
		m_boundingBox.max = position + SIZE_BASE / 2.0f + glm::vec2(0.0f, SIZE_FIRE.y);

		tSinceLastFireColorsUpdate = 0.0f;

		return true;
	}

	void Torch::destroy()
	{
		for (LineShape& line : m_fire)
		{
			line.destroy();
		}
		m_base.destroy();
	}

	void Torch::render() const
	{
		m_base.render();
		for (const LineShape& line : m_fire)
		{
			line.render();
		}
	}

	void Torch::update(float dt)
	{
		if (tSinceLastFireColorsUpdate > TIME_BETWEEN_FIRE_COLORS_UPDATES)
		{
			updateFireColors();
			tSinceLastFireColorsUpdate = 0.0f;
		}

		tSinceLastFireColorsUpdate += dt;
	}

	glm::vec2 Torch::getFirePosition() const
	{
		return
		{
			m_position.x,
			m_position.y + SIZE_BASE.y / 2.0f + SIZE_FIRE.y / 2.0f
		};
	}

	void Torch::updateFireColors()
	{
		for (LineShape& line : m_fire)
		{
			line.setColor(getRandomFireColor());
		}
	}

} // namespace GleamHouse
