#include "Torch.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "Renderer2DSystem.h"
#include "Player.h"

using namespace Pekan::Renderer2D;
using namespace Pekan::Utils;

namespace GleamHouse
{

	// Color of torch's base
	constexpr glm::vec4 COLOR_BASE = { 0.369f, 0.204f, 0.059f, 1.0f };
	// Size of torch's base, in world space
	constexpr glm::vec2 SIZE_BASE = { 0.15f, 0.6f };
	// Size of torch's fire, in world space
	constexpr glm::vec2 SIZE_FIRE = { SIZE_BASE.x * 1.5f, SIZE_BASE.y * 1.2f };
	// Total number of lines that torch's fire is made up of
	constexpr int COUNT_FIRE_LINES = 20;
	// Thickness of a line of fire
	constexpr float THICKNESS_FIRE_LINE = 0.05f;
	// Time between fire colors updates, in seconds
	constexpr float TIME_BETWEEN_FIRE_COLORS_UPDATES = 0.1f;

	constexpr float LIGHT_INTENSITY = 0.95f;
	constexpr float LIGHT_INTENSITY_AMPL = 0.05f;
	constexpr glm::vec3 LIGHT_COLOR = { 0.97f, 0.8f, 0.5f };
	constexpr glm::vec3 LIGHT_COLOR_AMPL = { 0.03f, 0.04f, 0.01f };
	constexpr float LIGHT_RADIUS = 2.0f;
	constexpr float LIGHT_RADIUS_AMPL = 0.05f;
	constexpr float LIGHT_SHARPNESS = 0.5f;
	constexpr float LIGHT_SHARPNESS_AMPL = 0.05f;

	// Offset from player's position to a torch's position when player holds the torch, in world space
	constexpr glm::vec2 OFFSET_FROM_PLAYER = glm::vec2(0.5f, 0.0f);

	std::vector<glm::vec4> getFireModel()
	{
		std::vector<glm::vec4> fireModel;
		fireModel.resize(COUNT_FIRE_LINES);

		// Offset between two consecutive lines in the middle of the fire
		const float lineOffset = SIZE_FIRE.x / float(COUNT_FIRE_LINES / 2 - 1);
		// Point where the bottom half of fire lines join together
		const glm::vec2 bottomJoinPoint = { 0.0f, SIZE_BASE.y / 2.0f };
		// Point where the top half of fire lines join together
		const glm::vec2 topJoinPoint = { bottomJoinPoint.x, bottomJoinPoint.y + SIZE_FIRE.y };
		// Y coordinate of fire's middle
		const float yMiddle = bottomJoinPoint.y + SIZE_FIRE.y / 2.0f;
		// X coordinate of fire's leftmost point
		const float xLeft = -SIZE_FIRE.x / 2.0f;

		// Create bottom half of fire
		for (int i = 0; i < COUNT_FIRE_LINES / 2; i++)
		{
			fireModel[i] =
			{
				bottomJoinPoint.x, bottomJoinPoint.y,
				xLeft + lineOffset * float(i), yMiddle + SIZE_FIRE.y / 100.0f
			};
		}
		// Create top half of fire
		for (int i = 0; i < COUNT_FIRE_LINES / 2; i++)
		{
			fireModel[i + COUNT_FIRE_LINES / 2] =
			{
				topJoinPoint.x, topJoinPoint.y,
				xLeft + lineOffset * float(i), yMiddle - SIZE_FIRE.y / 100.0f
			};
		}

		return fireModel;
	}

	// A list of coordinates of lines making up torch's fire,
	// in model space, where base's center is at (0, 0)
	const std::vector<glm::vec4> g_fireModel = getFireModel();

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
		m_base.setColor(COLOR_BASE);
		m_base.setPosition(position);

		// Create fire
		{
			PK_ASSERT_QUICK(m_fire.empty());
			m_fire.resize(COUNT_FIRE_LINES);

			for (int i = 0; i < COUNT_FIRE_LINES; i++)
			{
				m_fire[i].create
				(
					{ g_fireModel[i].x, g_fireModel[i].y },
					{ g_fireModel[i].z, g_fireModel[i].w }
				);
				m_fire[i].setColor(getRandomFireColor());
				m_fire[i].setThickness(THICKNESS_FIRE_LINE);
				m_fire[i].setPosition(m_base.getPosition());
			}
		}

		// Initialize light properties
		m_lightProperties.position = position;
		m_lightProperties.color = LIGHT_COLOR;
		m_lightProperties.intensity = LIGHT_INTENSITY;
		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		PK_ASSERT_QUICK(camera != nullptr);
		m_lightProperties.radius = camera->worldToWindowSize({ LIGHT_RADIUS, LIGHT_RADIUS }).x;
		m_lightProperties.sharpness = LIGHT_SHARPNESS;
		m_lightProperties.isStar = false;

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
		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		PK_ASSERT_QUICK(camera != nullptr);
		m_lightProperties.position = camera->worldToWindowPosition(getFirePosition());

		updateBase();
		updateFire();

		tSinceLastFireColorsUpdate += dt;
	}

	void Torch::grabByPlayer(const Player* player)
	{
		PK_ASSERT_QUICK(m_player == nullptr);
		PK_ASSERT_QUICK(player != nullptr);
		m_player = player;

		m_base.setParent(player->getTransformable2D());
		m_base.setPosition(OFFSET_FROM_PLAYER);
	}

	glm::vec2 Torch::getPosition() const
	{
		return m_base.getPositionInWorld();
	}

	glm::vec2 Torch::getFirePosition() const
	{
		// TODO: fix formula here, not exactly right
		return
		{
			m_base.getPositionInWorld().x,
			m_base.getPositionInWorld().y + SIZE_BASE.y / 2.0f + SIZE_FIRE.y / 2.0f
		};
	}

	void Torch::updateBase()
	{
		if (m_player == nullptr)
		{
			return;
		}

		//m_base.setRotation(m_player->getRotation());

		for (int i = 0; i < m_fire.size(); i++)
		{
			m_fire[i].setPosition(m_base.getPosition());
			m_fire[i].setRotation(m_base.getRotation());
		}
	}

	void Torch::updateFire()
	{
		if (tSinceLastFireColorsUpdate > TIME_BETWEEN_FIRE_COLORS_UPDATES)
		{
			// Update light properties with new random values
			{
				Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
				PK_ASSERT_QUICK(camera != nullptr);

				m_lightProperties.color = LIGHT_COLOR + getRandomFloat(-1.0f, 1.0f) * LIGHT_COLOR_AMPL;
				m_lightProperties.intensity = LIGHT_INTENSITY + getRandomFloat(-1.0f, 1.0f) * LIGHT_INTENSITY_AMPL;
				const float randomRadius = LIGHT_RADIUS + getRandomFloat(-1.0f, 1.0f) * LIGHT_RADIUS_AMPL;
				m_lightProperties.radius = camera->worldToWindowSize({ randomRadius, randomRadius }).x;;
				m_lightProperties.sharpness = LIGHT_SHARPNESS + getRandomFloat(-1.0f, 1.0f) * LIGHT_SHARPNESS_AMPL;
			}
			// Update fire colors with new random values
			for (int i = 0; i < m_fire.size(); i++)
			{
				m_fire[i].setColor(getRandomFireColor());
			}

			tSinceLastFireColorsUpdate = 0.0f;
		}
	}

} // namespace GleamHouse
