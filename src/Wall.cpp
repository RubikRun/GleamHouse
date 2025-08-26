#include "Wall.h"

#include "Image.h"
#include "Texture2D.h"

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace GleamHouse
{

	// Filepath of the image to be used for wall's sprite
	static constexpr char* IMAGE_FILEPATH = GLEAMHOUSE_ROOT_DIR "/src/resources/GleamHouse_wall.png";
	// Color of a wall's border
	static constexpr glm::vec4 BORDER_COLOR = { 0.2f, 0.0f, 0.0f, 1.0f };
	// Thickness of a wall's border
	static constexpr float BORDER_THICKNESS = 0.1f;
	// Texture scale of a wall, determining how many times the brick texture
	// will be repeated on a unit of distance.
	static constexpr float TEXTURE_SCALE = 2.5f;

	bool Wall::create(glm::vec2 bottomLeftPosition, glm::vec2 topRightPosition)
	{
		const glm::vec2 size = topRightPosition - bottomLeftPosition;
		const glm::vec2 centerPosition = bottomLeftPosition + size / 2.0f;
		// Create rectangle
		{
			m_rectangle.create(size.x + BORDER_THICKNESS, size.y + BORDER_THICKNESS);
			m_rectangle.setColor(BORDER_COLOR);
			m_rectangle.setPosition(centerPosition);
		}
		// Create sprite
		{
			// Load wall's image
			static Image image(IMAGE_FILEPATH);
			// Create a texture from wall's image
			std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
			texture->create(image);
			texture->setWrapModeX(TextureWrapMode::MirroredRepeat);
			texture->setWrapModeY(TextureWrapMode::MirroredRepeat);
			// Create wall's sprite using the texture
			m_sprite.create(texture, size.x, size.y);
			m_sprite.setTextureCoordinatesMax({ size.x * TEXTURE_SCALE, size.y * TEXTURE_SCALE });
			m_sprite.setPosition(centerPosition);
		}
		// Create bounding box
		m_boundingBox.min = bottomLeftPosition - glm::vec2(BORDER_THICKNESS, BORDER_THICKNESS);
		m_boundingBox.max = topRightPosition + glm::vec2(BORDER_THICKNESS, BORDER_THICKNESS);

		return true;
	}

	void Wall::destroy()
	{
		m_sprite.destroy();
		m_rectangle.destroy();
	}

	void Wall::render() const
	{
		m_rectangle.render();
		m_sprite.render();
	}

} // namespace GleamHouse
