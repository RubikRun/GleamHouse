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

	const float CAMERA_SCALE = 10.0f;

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

		if (!m_player.create())
		{
			PK_LOG_ERROR("Failed to create player.", "GleamHouse");
			return false;
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
	}

	void GleamHouse_Scene::render() const
	{
        Renderer2DSystem::beginFrame();
        RenderCommands::clear();

		m_floor.render();
		m_player.render();
#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		m_centerSquare.render();
#endif

        Renderer2DSystem::endFrame();
	}

	void GleamHouse_Scene::exit()
	{
#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		m_centerSquare.destroy();
#endif
		m_player.destroy();
		m_floor.destroy();
		m_camera->destroy();
	}

	void GleamHouse_Scene::createCamera()
	{
		m_camera = std::make_shared<Camera2D>();
		m_camera->create(CAMERA_SCALE);

		Renderer2DSystem::setCamera(m_camera);
		PekanTools::enableCameraController2D(m_camera);
		PekanTools::setCameraController2DZoomSpeed(1.1f);
	}

} // namespace GleamHouse