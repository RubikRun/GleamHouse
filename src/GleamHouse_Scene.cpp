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

	// Bottom-left positions of walls in level 1
	static constexpr glm::vec2 WALLS_BOTTOM_LEFT_POSITIONS[GleamHouse_Scene::WALLS_COUNT] =
	{
		{ -5.0f, 5.0f },
		{ -5.0f, -100.0f },
		{ -100.0f, -100.0f },
		{ 5.0f, -100.0f },
		{ 5.0f, 1.0f }
	};
	// Top-right positions of walls in level 1
	static constexpr glm::vec2 WALLS_TOP_RIGHT_POSITIONS[GleamHouse_Scene::WALLS_COUNT] =
	{
		{ 5.0f, 100.0f },
		{ 5.0f, -5.0f },
		{ -5.0f, 100.0f },
		{ 7.0f, -1.0f },
		{ 7.0f, 100.0f }
	};

    bool GleamHouse_Scene::init()
	{
        RenderState::enableMultisampleAntiAliasing();
		// Enable and configure blending
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		// Create floor
		if (!m_floor.create())
		{
			PK_LOG_ERROR("Failed to create floor.", "GleamHouse");
			return false;
		}
		// Create player
		if (!m_player.create())
		{
			PK_LOG_ERROR("Failed to create player.", "GleamHouse");
			return false;
		}
		// Create walls
		for (int i = 0; i < WALLS_COUNT; i++)
		{
			if (!m_walls[i].create(WALLS_BOTTOM_LEFT_POSITIONS[i], WALLS_TOP_RIGHT_POSITIONS[i]))
			{
				PK_LOG_ERROR("Failed to create a wall.", "GleamHouse");
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
		m_player.update();
		updateCamera();
	}

	void GleamHouse_Scene::render() const
	{
        Renderer2DSystem::beginFrame();
        RenderCommands::clear();

		m_floor.render();
		m_player.render();
		for (const Wall& wall : m_walls)
		{
			wall.render();
		}
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
		for (Wall& wall : m_walls)
		{
			wall.destroy();
		}
		m_player.destroy();
		m_floor.destroy();
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