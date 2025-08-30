#include "GleamHouse_Scene.h"

#include "PekanLogger.h"
#include "Renderer2DSystem.h"
#include "RenderState.h"
#include "PekanTools.h"
#include "PekanEngine.h"

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Tools;
using namespace Pekan;

namespace GleamHouse
{

	static constexpr float CAMERA_SCALE = 10.0f;
	// Interpolation factor to be used for camera's movement
	static constexpr float CAMERA_LERP_FACTOR = 0.05f;

	static constexpr glm::vec2 MAP_BOTTOM_LEFT_POSITION = { -100.0f, -100.0f };
	static constexpr glm::vec2 MAP_TOP_RIGHT_POSITION = { 100.0f, 100.0f };

	// Bottom-left positions of floors
	static constexpr glm::ivec2 FLOORS_BOTTOM_LEFT_POSITIONS[GleamHouse_Scene::FLOORS_COUNT] =
	{
		{ -5, -5 },
		{ 5, -1 },
		{ 12, -10 },
		{ 14, -10 },
		{ 14, 8 },
		{ 16, 10 },
		{ 18, 14 },
		{ 20, 14 },
		{ 44, 30 },
		{ 50, 14 },
		{ 54, 10 },
		{ 55, 20 },
		{ 64, 16 },
		{ 79, -10 },
		{ 80, -5 },
		{ 55, 10 },
		{ 56, 5 },
		{ 58, 0 },
		{ 58, -6 },
		{ 54, -6 },
		{ 22, -36 },
	};
	// Top-right positions of floors
	static constexpr glm::ivec2 FLOORS_TOP_RIGHT_POSITIONS[GleamHouse_Scene::FLOORS_COUNT] =
	{
		{ 5, 5 },
		{ 12, 1 },
		{ 14, 10 },
		{ 18, -8 },
		{ 24, 10 },
		{ 18, 16 },
		{ 20, 16 },
		{ 44, 32 },
		{ 68, 32 },
		{ 51, 30 },
		{ 55, 30 },
		{ 80, 21 },
		{ 65, 20 },
		{ 80, 20 },
		{ 88, -4 },
		{ 56, 11 },
		{ 70, 15 },
		{ 79, 1 },
		{ 59, 0 },
		{ 58, -5 },
		{ 54, 0 },
	};

    bool GleamHouse_Scene::init()
	{
        RenderState::enableMultisampleAntiAliasing();
		// Enable and configure blending
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		// Create background wall
		if (!m_wall.create(MAP_BOTTOM_LEFT_POSITION, MAP_TOP_RIGHT_POSITION))
		{
			PK_LOG_ERROR("Failed to create background wall.", "GleamHouse");
			return false;
		}
		// Create player
		if (!m_player.create())
		{
			PK_LOG_ERROR("Failed to create player.", "GleamHouse");
			return false;
		}
		// Create floor pieces
		for (int i = 0; i < FLOORS_COUNT; i++)
		{
			if (!m_floors[i].create(FLOORS_BOTTOM_LEFT_POSITIONS[i], FLOORS_TOP_RIGHT_POSITIONS[i]))
			{
				PK_LOG_ERROR("Failed to create a floor piece.", "GleamHouse");
				return false;
			}
		}

#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		// Create center square
		{
			m_centerSquare.create(0.2f, 0.2f);
			m_centerSquare.setColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
#endif

		createCamera();

        return true;
	}

	void GleamHouse_Scene::update(double dt)
	{
		m_player.update(m_floors, FLOORS_COUNT);
		updateCamera();
	}

	void GleamHouse_Scene::render() const
	{
        Renderer2DSystem::beginFrame();
        RenderCommands::clear();

		m_wall.render();
		for (const Floor& floor : m_floors)
		{
			floor.render();
		}
		m_player.render();
#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		m_centerSquare.render();
#endif

        Renderer2DSystem::endFrame();
	}

	void GleamHouse_Scene::exit()
	{
		m_camera->destroy();
#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		m_centerSquare.destroy();
#endif
		for (Floor& floor : m_floors)
		{
			floor.destroy();
		}
		m_player.destroy();
		m_wall.destroy();
	}

	void GleamHouse_Scene::createCamera()
	{
		m_camera = std::make_shared<Camera2D>();
		m_camera->create(CAMERA_SCALE);

		Renderer2DSystem::setCamera(m_camera);
	}

	void GleamHouse_Scene::updateCamera()
	{
		PK_ASSERT_QUICK(m_camera != nullptr);

		const glm::vec2 playerPos = m_player.getPosition();
		const glm::vec2 cameraPos = m_camera->getPosition();
		const glm::vec2 newCameraPos = cameraPos + (playerPos - cameraPos) * CAMERA_LERP_FACTOR;
		m_camera->setPosition(newCameraPos);
	}


} // namespace GleamHouse