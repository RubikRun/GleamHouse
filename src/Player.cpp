#include "Player.h"

#include "Image.h"
#include "Texture2D.h"
#include "PekanEngine.h"
#include "Renderer2DSystem.h"
#include "BoundingCircle.h"

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace GleamHouse
{

	// Filepath of the image to be used for player's sprite
	static constexpr char* IMAGE_FILEPATH = GLEAMHOUSE_ROOT_DIR "/src/resources/GleamHouse_player.png";
	// Player's size, in world space
	static constexpr float SIZE = 1.0f;
	// Player's speed, in world space, per frame
	static constexpr float SPEED = 0.05f;
	// Radius of player's bounding circle
	static constexpr float BOUNDING_CIRCLE_RADIUS = SIZE * 0.5f * 0.8f;

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

	void Player::update(const Wall* walls, int wallsCount)
	{
		// If W/A/S/D key is pressed move player up/left/down/right,
		// but only if it can be moved there.
		if (PekanEngine::isKeyPressed(KeyCode::KEY_W))
		{
			const glm::vec2 delta = { 0.0f, SPEED };
			if (canMoveBy(delta, walls, wallsCount))
			{
				m_sprite.move(delta);
			}
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_A))
		{
			const glm::vec2 delta = { -SPEED, 0.0f };
			if (canMoveBy(delta, walls, wallsCount))
			{
				m_sprite.move(delta);
			}
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_S))
		{
			const glm::vec2 delta = { 0.0f, -SPEED };
			if (canMoveBy(delta, walls, wallsCount))
			{
				m_sprite.move(delta);
			}
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_D))
		{
			const glm::vec2 delta = { SPEED, 0.0f };
			if (canMoveBy(delta, walls, wallsCount))
			{
				m_sprite.move(delta);
			}
		}

		// Rotate sprite to point towards the mouse
		{
			// Get mouse position, in world space
			const glm::vec2 mousePosition = Renderer2DSystem::getMousePosition();
			// Calculate vector from player to mouse
			const glm::vec2 playerToMouseVec = mousePosition - getPosition();
			// Calculate the angle that this vector makes with the positive X-axis
			const float angle = std::atan2f(playerToMouseVec.x, playerToMouseVec.y);
			// Set sprite's rotation to be that angle
			m_sprite.setRotation(angle);
		}
	}

	bool Player::canMoveBy(glm::vec2 delta, const Wall* walls, int wallsCount)
	{
		// Calculate player's position after applying the delta vector
		const glm::vec2 newPosition = getPosition() + delta;
		// Create bounding box for resulting position
		BoundingCircle boundingCircle;
		boundingCircle.position = newPosition;
		boundingCircle.radius = BOUNDING_CIRCLE_RADIUS;
		// Check if bounding circle collides with any wall
		for (int i = 0; i < wallsCount; i++)
		{
			const BoundingBox wallBoundingBox = walls[i].getBoundingBox();
			// If player's bounding circle collides with a wall's bounding box,
			// then we can't move player by the delta vector
			if (boundingCircle.collides(wallBoundingBox))
			{
				return false;
			}
		}

		return true;
	}

} // namespace GleamHouse
