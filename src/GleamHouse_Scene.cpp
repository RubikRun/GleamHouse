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

namespace Demo
{

	const float CAMERA_SCALE = 10.0f;

    bool GleamHouse_Scene::init()
	{
        if (m_guiWindow == nullptr)
        {
            PK_LOG_ERROR("There is no GUI window attached to GleamHouse_Scene.", "GleamHouse");
            return false;
        }

        RenderState::enableMultisampleAntiAliasing();

		m_centerSquare.create(1.0f, 1.0f);
		m_centerSquare.setColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		createCamera();

        return true;
	}

	void GleamHouse_Scene::update(double dt)
	{
	}

	void GleamHouse_Scene::render() const
	{
        Renderer2DSystem::beginFrame();

        glm::vec4 clearColor = m_guiWindow->getBackgroundColor();
        RenderState::setBackgroundColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        RenderCommands::clear();

		m_centerSquare.render();

        Renderer2DSystem::endFrame();
	}

	void GleamHouse_Scene::exit()
	{
		m_centerSquare.destroy();
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

} // namespace Demo