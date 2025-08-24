#include "Player.h"

#include "Image.h"
#include "Texture2D.h"
#include "PekanLogger.h"
#include "PekanEngine.h"

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace GleamHouse
{

	// Filepath of the image to be used for player's sprite
	static const char* IMAGE_FILEPATH = GLEAMHOUSE_ROOT_DIR "/src/resources/GleamHouse_player.png";
	// Player's size, in world space
	static const float SIZE = 1.0f;
	// Player's speed, in world space, per frame
	static const float SPEED = 0.05f;

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

	void Player::update()
	{
		// Move player up/left/down/right if W/A/S/D key is pressed
		if (PekanEngine::isKeyPressed(KeyCode::KEY_W))
		{
			m_sprite.move({ 0.0f, SPEED });
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_A))
		{
			m_sprite.move({ -SPEED, 0.0f });
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_S))
		{
			m_sprite.move({ 0.0f, -SPEED });
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_D))
		{
			m_sprite.move({ SPEED, 0.0f });
		}
	}

} // namespace GleamHouse
