#include "GleamHouse_Scene.h"

#include "PekanLogger.h"
#include "Renderer2DSystem.h"
#include "RenderState.h"
#include "PekanTools.h"
#include "PekanEngine.h"
#include "PostProcessor.h"
#include "FinishedLevel_Scene.h"
#include "LightProperties.h"
#include "Events/KeyEvents.h"

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Tools;
using namespace Pekan;

#define POST_PROCESSING_SHADER_FILEPATH "src/shaders/PostProcessingShader.glsl"
#define PI 3.14159265f

namespace GleamHouse
{

	static constexpr float CAMERA_SCALE = 10.0f;
	// Interpolation factor to be used for camera's movement
	static constexpr float CAMERA_LERP_FACTOR = 0.05f;

	static constexpr glm::vec2 MAP_BOTTOM_LEFT_POSITION = { -100.0f, -100.0f };
	static constexpr glm::vec2 MAP_TOP_RIGHT_POSITION = { 100.0f, 100.0f };

	// Bottom-left positions of floor pieces
	static constexpr glm::ivec2 FLOORS_BOTTOM_LEFT_POSITIONS[GleamHouse_Scene::FLOORS_COUNT] =
	{
		{ -5, -5 }, { 5, -1 }, { 12, -10 }, { 14, -10 }, { 14, 8 }, { 16, 10 }, { 18, 14 }, { 20, 14 },
		{ 44, 30 }, { 50, 14 }, { 54, 10 }, { 55, 18 }, { 64, 16 }, { 72, -10 }, { 73, -5 }, { 55, 10 },
		{ 56, 5 }, { 58, 2 }, { 58, -6 }, { 54, -6 }, { 22, -36 },
	};
	// Top-right positions of floor pieces
	static constexpr glm::ivec2 FLOORS_TOP_RIGHT_POSITIONS[GleamHouse_Scene::FLOORS_COUNT] =
	{
		{ 5, 5 }, { 12, 1 }, { 14, 10 }, { 18, -8 }, { 24, 10 }, { 18, 16 }, { 20, 16 }, { 44, 32 },
		{ 68, 32 }, { 51, 30 }, { 55, 30 }, { 72, 19 }, { 65, 18 }, { 73, 20 }, { 78, -4 }, { 56, 11 },
		{ 70, 15 }, { 72, 3 }, { 59, 2 }, { 58, -5 }, { 54, 0 },
	};
	// Color of bottom left tile of each floor piece
	static constexpr bool FLOORS_IS_BOTTOM_LEFT_BLACK[GleamHouse_Scene::FLOORS_COUNT] =
	{
		true, true, true, true, true, true, true, true,
		true, true, true, false, true, true, true, false,
		false, true, true, true, true
	};

	static constexpr glm::vec2 STAR_POSITION = { 20.0f, -40.0f };
	static constexpr float STAR_BASE_INTENSITY = 80.0f;
	static constexpr glm::vec3 STAR_BASE_COLOR = { 0.5f, 0.3f, 1.0f };

	static constexpr float TARGET_DIST_TO_STAR = 18.0f;

	static constexpr glm::vec2 TORCHES_POSITIONS[GleamHouse_Scene::TORCHES_COUNT] =
	{
		{ 23.5f, 8.5f },
		{ 56.5f, 14.5f },
		{ 69.5f, 5.5f },
		{ 72.5f, -4.5f }
	};

    bool GleamHouse_Scene::init()
	{
		PK_ASSERT_QUICK(m_finishedLevelScene != nullptr);

        RenderState::enableMultisampleAntiAliasing();
		// Enable and configure blending
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		createCamera();

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
			if (!m_floors[i].create(FLOORS_BOTTOM_LEFT_POSITIONS[i], FLOORS_TOP_RIGHT_POSITIONS[i], FLOORS_IS_BOTTOM_LEFT_BLACK[i]))
			{
				PK_LOG_ERROR("Failed to create a floor piece.", "GleamHouse");
				return false;
			}
		}

		// Create torches
		for (int i = 0; i < TORCHES_COUNT; i++)
		{
			if (!m_torches[i].create(TORCHES_POSITIONS[i]))
			{
				PK_LOG_ERROR("Failed to create a torch.", "GleamHouse");
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

		if (!PostProcessor::init(POST_PROCESSING_SHADER_FILEPATH))
		{
			PK_LOG_ERROR("Failed to initialize PostProcessor", "Demo06");
		}

		t = 0.0f;

        return true;
	}

	void GleamHouse_Scene::update(double dt)
	{
		updateCamera();
		m_player.update(m_floors, FLOORS_COUNT);
		for (int i = 0; i < TORCHES_COUNT; i++)
		{
			m_torches[i].update(float(dt));
		}
		updateDistToStar();
		updateLights();

		if (m_distToStar < TARGET_DIST_TO_STAR)
		{
			m_player.setIsPlayable(false);
			m_hasFinished = true;
			m_finishedLevelScene->play();
		}

		if (m_hasFinished)
		{
			if (m_finishedLevelScene->getTime() > 3.0f)
			{
				static bool stoppedRotating = false;
				if (!stoppedRotating)
				{
					if (m_player.isFacingRight())
					{
						stoppedRotating = true;
						m_player.setRotation(-PI / 2.0f);
					}
					else
					{
						m_player.rotate(float(dt));
					}
				}
			}
			else
			{
				m_player.rotate(float(dt));
			}
		}

		t += float(dt);
	}

	void GleamHouse_Scene::render() const
	{
		PostProcessor::beginFrame();
        Renderer2DSystem::beginFrame();
        RenderCommands::clear();

		m_wall.render();
		for (const Floor& floor : m_floors)
		{
			floor.render();
		}
		m_player.render();
		for (int i = 0; i < TORCHES_COUNT; i++)
		{
			m_torches[i].render();
		}
#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		m_centerSquare.render();
#endif

        Renderer2DSystem::endFrame();
		PostProcessor::endFrame();
	}

	void GleamHouse_Scene::exit()
	{
#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		m_centerSquare.destroy();
#endif
		for (int i = 0; i < TORCHES_COUNT; i++)
		{
			m_torches[i].destroy();
		}
		for (Floor& floor : m_floors)
		{
			floor.destroy();
		}
		m_player.destroy();
		m_wall.destroy();
		m_camera->destroy();
	}

	glm::vec2 GleamHouse_Scene::getPlayerSizeNDC() const
	{
		const glm::vec2 playerSize = m_player.getSize();
		return m_camera->worldToNdcSize(playerSize);
	}

	bool GleamHouse_Scene::onKeyPressed(const KeyPressedEvent& event)
	{
		if (event.getKeyCode() == KeyCode::KEY_E)
		{
			if (!m_player.hasTorch())
			{
				for (Torch& torch : m_torches)
				{
					if (m_player.canGrabTorch(torch))
					{
						m_player.grabTorch(torch);
						break;
					}
				}
			}
			return true;
		}
		else if (event.getKeyCode() == KeyCode::KEY_Q)
		{
			if (m_player.hasTorch())
			{
				m_player.dropTorch();
			}
		}

		return false;
	}

	void GleamHouse_Scene::createCamera()
	{
		m_camera = std::make_shared<Camera2D>();
		m_camera->create(CAMERA_SCALE);

		// TEMP
		//m_camera->setZoom(0.1f);

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

	// Updates post-processing shader with given list of lights
	static void updatePostProcessingShader(const LightProperties* lights, int lightsCount)
	{
		Shader* ppShader = PostProcessor::getShader();
		ppShader->setUniform1i("uLightsCount", lightsCount);

		std::vector<glm::vec2> positions(lightsCount, { 0.0f, 0.0f });
		std::vector<glm::vec3> colors(lightsCount, { 1.0f, 1.0f, 1.0f });
		std::vector<float> intensities(lightsCount, 1.0f);
		std::vector<float> radii(lightsCount, 10.0f);
		std::vector<float> sharpnesses(lightsCount, 0.5f);
		std::vector<float> isStar(lightsCount, 0.0f);
		for (int i = 0; i < lightsCount; i++)
		{
			positions[i] = lights[i].position;
			colors[i] = lights[i].color;
			intensities[i] = lights[i].intensity;
			radii[i] = lights[i].radius;
			sharpnesses[i] = lights[i].sharpness;
			isStar[i] = (lights[i].isStar ? 1.0f : 0.0f);
		}

		ppShader->setUniform2fv("uLightPositions", positions.size(), positions.data());
		ppShader->setUniform3fv("uLightColors", colors.size(), colors.data());
		ppShader->setUniform1fv("uLightIntensities", intensities.size(), intensities.data());
		ppShader->setUniform1fv("uLightRadii", radii.size(), radii.data());
		ppShader->setUniform1fv("uLightSharpnesses", sharpnesses.size(), sharpnesses.data());
		ppShader->setUniform1fv("uLightIsStarFlags", isStar.size(), isStar.data());

		const glm::vec2 resolution = glm::vec2(PekanEngine::getWindow().getSize());
		ppShader->setUniform2f("uResolution", resolution);
	}

	void GleamHouse_Scene::updateLights()
	{
		PK_ASSERT(m_camera != nullptr, "Cannot update lights because camera is null.", "Demo06");
		PK_ASSERT_QUICK(TORCHES_COUNT >= 0);

		static LightProperties lights[TORCHES_COUNT + 1];

		lights[0].position = m_camera->worldToWindowPosition(STAR_POSITION);
		lights[0].color = getStarColor();
		lights[0].intensity = getStarIntensity();
		lights[0].radius = m_camera->worldToWindowSize({5.7f, 5.7f}).x;
		lights[0].sharpness = 0.1f;
		lights[0].isStar = true;

		for (int i = 0; i < TORCHES_COUNT; i++)
		{
			lights[i + 1] = m_torches[i].getLightProperties();
		}

		updatePostProcessingShader(lights, TORCHES_COUNT + 1);
	}

	void GleamHouse_Scene::updateDistToStar()
	{
		const glm::vec2 playerPos = m_player.getPosition();
		const glm::vec2 starToPlayerVec = playerPos - STAR_POSITION;
		m_distToStar = std::sqrtf(starToPlayerVec.x * starToPlayerVec.x + starToPlayerVec.y * starToPlayerVec.y);
	}

	float GleamHouse_Scene::getStarIntensity()
	{
		if (m_distToStar > 30.0f)
		{
			return STAR_BASE_INTENSITY;
		}
		return (0.01f * (30.0f - m_distToStar) * (30.0f - m_distToStar) + 1.0f) * STAR_BASE_INTENSITY;
	}

	glm::vec3 GleamHouse_Scene::getStarColor()
	{
		if (m_distToStar > 30.0f)
		{
			return STAR_BASE_COLOR;
		}
		if (m_distToStar < 10.0f)
		{
			return glm::vec3(1.0f, 1.0f, 1.0f);
		}
		const float inter = (30.0f - m_distToStar) / (30.0f - 10.0f);
		return (1.0f - inter) * STAR_BASE_COLOR + inter * glm::vec3(1.0f, 1.0f, 1.0f);
	}

} // namespace GleamHouse