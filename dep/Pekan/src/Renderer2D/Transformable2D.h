#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	class Transformable2D
	{
	public:

		void setParent(const Transformable2D* parent);

		void setPosition(glm::vec2 position);
		void setRotation(float rotation); // in radians
		void setScale(glm::vec2 scale);

		void move(glm::vec2 deltaPosition);
		void rotate(float deltaRotation);
		void scale(glm::vec2 deltaScale);

		glm::vec2 getPosition() const { return m_position; }
		glm::vec2 getPositionInWorld() const;
		float getRotation() const { return m_rotation; }
		float getRotationInWorld() const;
		glm::vec2 getScale() const { return m_scale; }
		glm::vec2 getScaleInWorld() const;

		const glm::mat3& getLocalMatrix() const;
		const glm::mat3& getWorldMatrix() const;

		unsigned getChangeId() const;

	protected:

		void _create();
		void _destroy();

	private: /* functions */

		void updateLocalMatrix() const;
		void updateWorldMatrix() const;

		void registerLocalChange() const;
		void registerParentChange() const;

	private: /* variables */

		glm::vec2 m_position = glm::vec2(-1.0f, -1.0f);
		float m_rotation = -1.0f; // in radians
		glm::vec2 m_scale = glm::vec2(-1.0f, -1.0f);

		const Transformable2D* m_parent = nullptr;

		mutable glm::mat3 m_localMatrix = glm::mat3(1.0f);
		mutable glm::mat3 m_worldMatrix = glm::mat3(1.0f);

		mutable bool m_isDirtyLocalMatrix = true;
		mutable bool m_isDirtyWorldMatrix = true;

		mutable unsigned m_changeId = 0;
		mutable unsigned m_cachedParentChangeId = 0;
		mutable unsigned m_parentChangeIdUsedInWorldMatrix = 0;
	};

} // namespace Renderer2D
} // namespace Pekan
