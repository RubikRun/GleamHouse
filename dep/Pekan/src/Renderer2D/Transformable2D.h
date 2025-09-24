#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A base class for 2D transformable objects
	class Transformable2D
	{
	public:

		// Sets another transformable object to be the parent of this one.
		//
		// NOTE: Null can be passed to make the object NOT have a parent.
		//       By default objects do NOT have a parent.
		void setParent(const Transformable2D* parent);

		// Sets a new local position to the object
		void setPosition(glm::vec2 position);
		// Sets a new local rotation to the object
		void setRotation(float rotation); // in radians
		// Sets a new local scale to the object
		void setScale(glm::vec2 scale);

		// Moves the object, changing its local position by some amount
		void move(glm::vec2 deltaPosition);
		// Rotates the object, changing its local rotation by some amount
		void rotate(float deltaRotation);
		// Scales the object, changing its local scale by some amount
		void scale(glm::vec2 deltaScale);

		// Returns object's local position (relative to parent's space)
		glm::vec2 getPosition() const { return m_position; }
		// Returns object's world position
		glm::vec2 getPositionInWorld() const;
		// Returns object's local rotation (relative to parent's space)
		float getRotation() const { return m_rotation; }
		// Returns object's world rotation
		float getRotationInWorld() const;
		// Returns object's local scale (relative to parent's space)
		glm::vec2 getScale() const { return m_scale; }
		// Returns object's world scale
		glm::vec2 getScaleInWorld() const;

		// Returns object's local transform matrix containing the combined action
		// of transforming the object with its position, rotation and scale
		const glm::mat3& getLocalMatrix() const;
		// Returns object's world transform matrix containing the combined action
		// of transforming the object, first with its parent's transform,
		// then with its own position, rotation and scale
		const glm::mat3& getWorldMatrix() const;

	protected:

		// Creates a 2D transformable object.
		//
		// NOTE: Cannot be used directly on this class.
		//       To be used by derived classes' create() function.
		void _create();
		// Destroys a 2D transformable object.
		//
		// NOTE: Cannot be used directly on this class.
		//       To be used by derived classes' create() function.
		void _destroy();

	private: /* functions */

		// Updates the local matrix with current position, rotation and scale
		void updateLocalMatrix() const;
		// Updates the world matrix with current position, rotation and scale, and with current parent's transform.
		void updateWorldMatrix() const;

	private: /* variables */

		// Transformable object's parent.
		// A transformable object is transformed relative to its parent's transform.
		// In other words, child's transform is applied first and then the parent's transform is applied on top of that.
		const Transformable2D* m_parent = nullptr;

		// Position, rotation and scale of the transformable object,
		// used to transform the object from local space (parent's space) to world space
		glm::vec2 m_position = glm::vec2(-1.0f, -1.0f);
		float m_rotation = -1.0f; // in radians
		glm::vec2 m_scale = glm::vec2(-1.0f, -1.0f);

		// A 2D transform matrix, containing the combined action
		// of transforming the object with its scale, rotation and position (in this order).
		mutable glm::mat3 m_localMatrix = glm::mat3(1.0f);
		// A 2D transform matrix, containing the combined action
		// of transforming the object, first with its parent's transform,
		// then with its own scale, rotation and position (in this order).
		mutable glm::mat3 m_worldMatrix = glm::mat3(1.0f);

		// Flag indicating if local matrix has to be updated before use
		mutable bool m_needUpdateLocalMatrix = false;
		// Flag indicating if world matrix has to be updated before use
		mutable bool m_needUpdateWorldMatrix = false;
	};

} // namespace Renderer2D
} // namespace Pekan
