#include "Player.h"

#include "Image.h"
#include "Texture2D.h"
#include "PekanLogger.h"

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace GleamHouse
{

	// Filepath of the image to be used for player's sprite
	static const char* IMAGE_FILEPATH = GLEAMHOUSE_ROOT_DIR "/src/resources/GleamHouse_player.png";
	// Player's size, in world space
	static const float SIZE = 1.0f;

	bool Player::create()
	{
		// Create sprite
		{
			// Load player's image
			Image image(IMAGE_FILEPATH);
			// Create a texture from player's image
			std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
			texture->create(image);
			// Create player's sprite using the texture
			m_sprite.create(texture, SIZE, SIZE);
		}

		return true;
	}

	void Player::destroy()
	{
		m_sprite.destroy();
	}

	void Player::render() const
	{
		m_sprite.render();
	}

} // namespace GleamHouse
