#include "Player.h"

#include "Image.h"
#include "Texture2D.h"
#include "PekanEngine.h"
#include "Renderer2DSystem.h"
#include "BoundingCircle.h"
#include "PekanLogger.h"
#include "Torch.h"

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace GleamHouse
{

	// Filepath of the image to be used for player's sprite
	static constexpr char* IMAGE_FILEPATH = GLEAMHOUSE_ROOT_DIR "/src/resources/GleamHouse_player.png";
	// Player's size, in world space
	static constexpr float SIZE = 1.0f;
	static constexpr glm::vec2 SIZE_VEC = { SIZE, SIZE };
	// Player's speed, in world space, per frame
	static constexpr float SPEED = 0.05f;
	// Radius of player's bounding circle
	static constexpr float BOUNDING_CIRCLE_RADIUS = SIZE * 0.5f * 1.2f;
	// Number of subdivisions, per dimension, to use for doing collision checks with player's bounding circle
	static constexpr int BOUNDING_CIRCLE_SUBDIVS = 8;
	// Maximum number of floor pieces that player can collide with at once
	static constexpr int MAX_FLOOR_COLLISIONS = 6;
	// Distance squared between torch and player when player can grab torch
	static constexpr float DIST_SQ_CAN_GRAB_TORCH = 1.0f;

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

		// TEMP

		// Uncomment next line to spawn close to level's end
		//m_sprite.setPosition({ 40.0f, -25.0f });

		// Uncomment next line to spawn close to first torch
		//m_sprite.setPosition({ 16.0f, 9.0f });

		// Uncomment next line to spawn in first big square
		//m_sprite.setPosition({ 30.0f, 20.0f });

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

	void Player::update(const Floor* floors, int floorsCount)
	{
		if (!m_isPlayable)
		{
			return;
		}

		// If W/A/S/D key is pressed move player up/left/down/right,
		// but only if it can be moved there.
		if (PekanEngine::isKeyPressed(KeyCode::KEY_W))
		{
			const glm::vec2 delta = { 0.0f, SPEED };
			if (canMoveBy(delta, floors, floorsCount))
			{
				m_sprite.move(delta);
			}
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_A))
		{
			const glm::vec2 delta = { -SPEED, 0.0f };
			if (canMoveBy(delta, floors, floorsCount))
			{
				m_sprite.move(delta);
			}
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_S))
		{
			const glm::vec2 delta = { 0.0f, -SPEED };
			if (canMoveBy(delta, floors, floorsCount))
			{
				m_sprite.move(delta);
			}
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_D))
		{
			const glm::vec2 delta = { SPEED, 0.0f };
			if (canMoveBy(delta, floors, floorsCount))
			{
				m_sprite.move(delta);
			}
		}

		if (isInNarrowTunnel(floors, floorsCount) && hasTorch())
		{
			dropTorch();
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

	glm::vec2 Player::getSize() const
	{
		return { SIZE, SIZE };
	}

	void Player::setIsPlayable(bool isPlayable)
	{
		m_isPlayable = isPlayable;
	}

	bool Player::isFacingRight() const
	{
		const float angle = m_sprite.getRotation();
		const float cosAngle = std::cosf(angle);
		const float sinAngle = std::sinf(angle);
		// Since player's texture is facing down by default,
		// it will be facing right when sprite's angle is pointing up,
		// which is equivalent to cos(angle) = 0 and sin(angle) = 1
		return (std::fabsf(cosAngle) < 0.01f && std::fabsf(sinAngle + 1.0f) < 0.01f);
	}

	bool Player::canGrabTorch(const Torch& torch) const
	{
		const glm::vec2 torchPos = torch.getPosition();
		const glm::vec2 playerPos = getPosition();
		const float distSq = (torchPos.x - playerPos.x) * (torchPos.x - playerPos.x)
			+ (torchPos.y - playerPos.y) * (torchPos.y - playerPos.y);

		return distSq < DIST_SQ_CAN_GRAB_TORCH;
	}

	void Player::grabTorch(Torch& torch)
	{
		m_torch = &torch;
		m_torch->onGrabbedByPlayer(this);
	}

	bool Player::hasTorch() const
	{
		return m_torch != nullptr;
	}

	void Player::dropTorch()
	{
		PK_ASSERT_QUICK(hasTorch());
		m_torch->onDroppedByPlayer();
		m_torch = nullptr;
	}

	bool Player::isInNarrowTunnel(const Floor* floors, int floorsCount) const
	{
		const glm::vec2 playerPos = getPosition();
		if (!isPointInFloor(playerPos, floors, floorsCount))
		{
			PK_LOG_WARNING("Checking if player is in a narrow tunnel but they're not even on the floor.", "GleamHouse");
			return false;
		}

		const glm::vec2 leftPoint = playerPos + glm::vec2(-1.0f, 0.0f);
		const glm::vec2 rightPoint = playerPos + glm::vec2(1.0f, 0.0f);
		const glm::vec2 downPoint = playerPos + glm::vec2(0.0f, -1.0f);
		const glm::vec2 upPoint = playerPos + glm::vec2(0.0f, 1.0f);

		return (!isPointInFloor(leftPoint, floors, floorsCount) && !isPointInFloor(rightPoint, floors, floorsCount))
		    || (!isPointInFloor(downPoint, floors, floorsCount) && !isPointInFloor(upPoint, floors, floorsCount));
	}

	bool Player::canMoveBy(glm::vec2 delta, const Floor* floors, int floorsCount)
	{
		// Calculate player's position after applying the delta vector
		const glm::vec2 newPosition = getPosition() + delta;

		// Create bounding box around the new position
		BoundingBox boundingBox;
		boundingBox.min = newPosition - SIZE_VEC / 2.0f;
		boundingBox.max = newPosition + SIZE_VEC / 2.0f;
		// List of indices of floor pieces that collide with player's bounding box
		int collisionIndices[MAX_FLOOR_COLLISIONS];
		int collisionsCount = 0;
		// Find floor pieces that collide with player's bounding box
		for (int i = 0; i < floorsCount; i++)
		{
			const BoundingBox floorBoundingBox = floors[i].getBoundingBox();
			if (boundingBox.collides(floorBoundingBox))
			{
				if (collisionsCount + 1 >= MAX_FLOOR_COLLISIONS)
				{
					PK_LOG_ERROR("Player collides with unexpectedly many floor pieces at once.", "GleamHouse");
					break;
				}
				collisionIndices[collisionsCount++] = i;
			}
		}
		// If there are no collided floor pieces, then player is fully outside of the floor so they can't move
		if (collisionsCount <= 0)
		{
			return false;
		}

		// Create bounding circle around the new position
		BoundingCircle boundingCircle;
		boundingCircle.position = newPosition;
		boundingCircle.radius = BOUNDING_CIRCLE_RADIUS;
		const glm::vec2 bcircBottomLeft = boundingCircle.position - boundingCircle.radius;
		// Subdivide bounding circle,
		// and look for a subdivision point that is NOT inside of any of the collided floor pieces
		for (int subdivRow = 0; subdivRow < BOUNDING_CIRCLE_SUBDIVS; subdivRow++)
		{
			for (int subdivCol = 0; subdivCol < BOUNDING_CIRCLE_SUBDIVS; subdivCol++)
			{
				// Calculate subdivision point, in world space
				const glm::vec2 subdivPoint =
				{
					bcircBottomLeft.x + boundingCircle.radius * 2.0f * float(subdivCol) / float(BOUNDING_CIRCLE_SUBDIVS - 1),
					bcircBottomLeft.y + boundingCircle.radius * 2.0f * float(subdivRow) / float(BOUNDING_CIRCLE_SUBDIVS - 1)
				};
				// If subdivision point is NOT inside the bounding circle, we can skip that point
				if (!boundingCircle.isPointInside(subdivPoint))
				{
					continue;
				}
				// If subdivision point is NOT inside any of the collided floor pieces,
				// than we can conclude that the bounding circle is NOT fully within the floor.
				bool inFloor = false;
				for (int i = 0; i < collisionsCount; i++)
				{
					PK_ASSERT_QUICK(collisionIndices[i] >= 0 && collisionIndices[i] < floorsCount);
					const BoundingBox floorBoundingBox = floors[collisionIndices[i]].getBoundingBox();
					if (floorBoundingBox.isPointInside(subdivPoint))
					{
						inFloor = true;
						break;
					}
				}
				if (!inFloor)
				{
					return false;
				}
			}
		}

		return true;
	}

	bool Player::isPointInFloor(glm::vec2 point, const Floor* floors, int floorsCount) const
	{
		for (int i = 0; i < floorsCount; i++)
		{
			const BoundingBox floorBoundingBox = floors[i].getBoundingBox();
			if (floorBoundingBox.isPointInside(point))
			{
				return true;
			}
		}
		return false;
	}

} // namespace GleamHouse
