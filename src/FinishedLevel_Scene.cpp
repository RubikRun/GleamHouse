#include "FinishedLevel_Scene.h"

#include "PekanLogger.h"
#include "Image.h"
#include "GleamHouse_Scene.h"
#include "Utils/FileUtils.h"

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan;

#define IMAGE_FILEPATH "src/resources/GleamHouse_FnishedLevel1.png"
#define VERTEX_SHADER_FILEPATH "src/shaders/FinishedLevel_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH "src/shaders/FinishedLevel_FragmentShader.glsl"

namespace GleamHouse
{

    bool FinishedLevel_Scene::init()
	{
		PK_ASSERT_QUICK(m_mainScene != nullptr);

        RenderState::enableMultisampleAntiAliasing();
		// Enable and configure blending
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		const glm::vec2 playerSize = m_mainScene->getPlayerSizeNDC();
		// Size of "finished level" rectangle, in NDC space
		const glm::vec2 rectangleSize = { 5.8f * playerSize.x, 3.3f * playerSize.y };
		// Position of "finished level" rectangle, in NDC space
		const glm::vec2 rectanglePosition = { 0.21f, -0.17f };

		float vertexData[16] =
		{	// aPosition                                                                                   // aTexCoord
			rectanglePosition.x - rectangleSize.x / 2.0f, rectanglePosition.y - rectangleSize.y / 2.0f,    0.0f, 0.0f,
			rectanglePosition.x + rectangleSize.x / 2.0f, rectanglePosition.y - rectangleSize.y / 2.0f,    1.0f, 0.0f,
			rectanglePosition.x + rectangleSize.x / 2.0f, rectanglePosition.y + rectangleSize.y / 2.0f,    1.0f, 1.0f,
			rectanglePosition.x - rectangleSize.x / 2.0f, rectanglePosition.y + rectangleSize.y / 2.0f,    0.0f, 1.0f
		};
		unsigned indexData[6] = { 0, 1, 2, 0, 2, 3 };

		m_renderObject.create
		(
			vertexData,
			16 * sizeof(float),
			{ { ShaderDataType::Float2, "aPosition" }, { ShaderDataType::Float2, "aTexCoord" } },
			BufferDataUsage::StaticDraw,
			FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);
		m_renderObject.setIndexData(indexData, 6 * sizeof(unsigned));
		const Image image(IMAGE_FILEPATH);
		m_renderObject.setTextureImage(image, "uTexture", 0);

		m_isPlaying = false;
		t = 0.0f;

        return true;
	}

	void FinishedLevel_Scene::update(double dt)
	{
		if (!m_isPlaying)
		{
			return;
		}

		m_renderObject.getShader().setUniform1f("uOpacity", t / 6.0f);

		t += float(dt);
	}

	void FinishedLevel_Scene::render() const
	{
		if (!m_isPlaying)
		{
			return;
		}
		m_renderObject.render();
	}

	void FinishedLevel_Scene::exit()
	{
		m_renderObject.destroy();
	}

} // namespace GleamHouse