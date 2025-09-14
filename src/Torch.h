#pragma once

#include "RectangleShape.h"
#include "LineShape.h"
#include "BoundingBox.h"
#include <vector>
#include "LightProperties.h"

namespace GleamHouse
{

	// A class representing a torch in Gleam House
	class Torch
	{

	public:

		// @param[in] position - position of the center of torch's base, in world space
		bool create(glm::vec2 position);
		void destroy();

		void render() const;

		void update(float dt);

		// Returns torch's bounding box, determining how the torch will collide with other objects
		BoundingBox getBoundingBox() const { return m_boundingBox; }

		// Returns position of the center of torch's fire, in world space
		glm::vec2 getFirePosition() const;

		// Returns light properties of torch's fire in current moment
		LightProperties getLightProperties() const { return m_lightProperties; }

	private: /* functions */

		void updateFireColors();
		void updateLightProperties();

	private: /* variables */

		// Position of the center of torch's base, in world space
		glm::vec2 m_position = { -1.0f, -1.0f };

		// Torch's rectangular base
		Pekan::Renderer2D::RectangleShape m_base;
		// A list of lines making up torch's fire
		std::vector<Pekan::Renderer2D::LineShape> m_fire;

		LightProperties m_lightProperties;

		// Time passed since last fire colors update
		float tSinceLastFireColorsUpdate = 0.0f;

		// Torch's bounding box, determining how the torch will collide with other objects
		BoundingBox m_boundingBox;
	};

} // namespace GleamHouse