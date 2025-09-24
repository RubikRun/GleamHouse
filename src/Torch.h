#pragma once

#include "RectangleShape.h"
#include "LineShape.h"
#include "BoundingBox.h"
#include <vector>
#include "LightProperties.h"

namespace GleamHouse
{

	class Player;

	// A class representing a torch in Gleam House
	class Torch
	{

	public:

		bool create(glm::vec2 position);
		void destroy();

		void render() const;

		void update(float dt);

		void grabByPlayer(const Player* player);

		glm::vec2 getPosition() const;

		// Returns position of the center of torch's fire, in world space
		glm::vec2 getFirePosition() const;

		// Returns light properties of torch's fire in current moment
		LightProperties getLightProperties() const { return m_lightProperties; }

	private: /* functions */

		void updateBase();
		void updateFire();

	private: /* variables */

		const Player* m_player = nullptr;

		glm::vec2 m_position = { 0.0f, 0.0f };

		// Torch's rectangular base
		Pekan::Renderer2D::RectangleShape m_base;
		// A list of lines making up torch's fire
		std::vector<Pekan::Renderer2D::LineShape> m_fire;

		LightProperties m_lightProperties;

		// Time passed since last fire colors update
		float tSinceLastFireColorsUpdate = 0.0f;
	};

} // namespace GleamHouse