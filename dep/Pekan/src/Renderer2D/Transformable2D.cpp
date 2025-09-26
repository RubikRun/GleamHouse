#include "Transformable2D.h"

#include "PekanLogger.h"
#include "glm/gtc/epsilon.hpp"

namespace Pekan
{
namespace Renderer2D
{

    void Transformable2D::setParent(const Transformable2D* parent)
    {
        m_parent = parent;
        m_changeId++;
    }

    void Transformable2D::setPosition(glm::vec2 position)
    {
        m_position = position;
        m_changeId++;
        m_isDirtyLocalMatrix = true;
    }

    void Transformable2D::setRotation(float rotation)
    {
        m_rotation = rotation;
        m_changeId++;
        m_isDirtyLocalMatrix = true;
    }

    void Transformable2D::setScale(glm::vec2 scale)
    {
        m_scale = scale;
        m_changeId++;
        m_isDirtyLocalMatrix = true;
    }

    void Transformable2D::move(glm::vec2 deltaPosition)
    {
        m_position += deltaPosition;
        m_changeId++;
        m_isDirtyLocalMatrix = true;
    }

    void Transformable2D::rotate(float deltaRotation)
    {
        m_rotation += deltaRotation;
        m_changeId++;
        m_isDirtyLocalMatrix = true;
    }

    void Transformable2D::scale(glm::vec2 deltaScale)
    {
        m_scale *= deltaScale;
        m_changeId++;
        m_isDirtyLocalMatrix = true;
    }

    glm::vec2 Transformable2D::getPositionInWorld() const
    {
        if (m_parent == nullptr)
        {
            return m_position;
        }
        const glm::mat3 parentWorldMatrix = m_parent->getWorldMatrix();
        const glm::vec2 positionInWorld = glm::vec2(parentWorldMatrix * glm::vec3(m_position, 1.0f));
        return positionInWorld;
    }

    float Transformable2D::getRotationInWorld() const
    {
        if (m_parent == nullptr)
        {
            return m_rotation;
        }
        const float rotationInWorld = m_parent->getRotationInWorld() + m_rotation;
        return rotationInWorld;
    }

    glm::vec2 Transformable2D::getScaleInWorld() const
    {
        glm::mat3 worldMatrix = getWorldMatrix();

        const glm::vec2 xAxis = glm::vec2(worldMatrix[0][0], worldMatrix[1][0]);
        const glm::vec2 yAxis = glm::vec2(worldMatrix[0][1], worldMatrix[1][1]);
        const float scaleX = glm::length(xAxis);
        const float scaleY = glm::length(yAxis);

        return { scaleX, scaleY };
    }

    const glm::mat3& Transformable2D::getLocalMatrix() const
    {
        if (m_isDirtyLocalMatrix)
        {
            updateLocalMatrix();
        }
        return m_localMatrix;
    }

    const glm::mat3 Transformable2D::getWorldMatrix() const
    {
        const glm::mat3& localMatrix = getLocalMatrix();

        if (m_parent != nullptr)
        {
            const glm::mat3 parentWorldMatrix = m_parent->getWorldMatrix();
            const glm::mat3 worldMatrix = parentWorldMatrix * localMatrix;
            return worldMatrix;
        }
        else
        {
            return localMatrix;
        }
    }

    unsigned Transformable2D::getChangeId() const
    {
        if (m_parent != nullptr && m_cachedParentChangeId < m_parent->m_changeId)
        {
            m_cachedParentChangeId = m_parent->m_changeId;
            m_changeId++;
        }

        return m_changeId;
    }

    void Transformable2D::_create()
    {
        // Set default values
        m_parent = nullptr;
        m_position = glm::vec2(0.0f, 0.0f);
        m_rotation = 0.0f;
        m_scale = glm::vec2(1.0f, 1.0f);
        m_changeId = 0;
        m_cachedParentChangeId = 0;
        m_localMatrix = glm::mat3(1.0f);
        m_isDirtyLocalMatrix = true;
    }

    void Transformable2D::_destroy()
    {}

    void Transformable2D::updateLocalMatrix() const
    {
        const float cosRot = cos(m_rotation);
        const float sinRot = sin(m_rotation);

        const glm::mat3 scaleMatrix = glm::mat3
        (
            glm::vec3(m_scale.x, 0.0f, 0.0f),
            glm::vec3(0.0f, m_scale.y, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        const glm::mat3 rotationMatrix = glm::mat3
        (
            glm::vec3(cosRot, -sinRot, 0.0f),
            glm::vec3(sinRot, cosRot, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        const glm::mat3 translationMatrix = glm::mat3
        (
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(m_position.x, m_position.y, 1.0f)
        );

        m_localMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        m_isDirtyLocalMatrix = false;
    }

} // namespace Renderer2D
} // namespace Pekan