#include "BoundingCircle.h"

#include "BoundingBox.h"

namespace GleamHouse
{

	bool BoundingCircle::collides(BoundingCircle other) const
    {
        const glm::vec2 positionDelta = position - other.position;
        const float radiusSum = radius + other.radius;
        return positionDelta.x * positionDelta.x + positionDelta.y * positionDelta.y <= radiusSum * radiusSum;
    }

    bool BoundingCircle::collides(const BoundingBox& box) const
    {
        return box.collides(*this);
    }

    bool BoundingCircle::isFullyInsideBox(const BoundingBox& box) const
    {
        return position.x - radius >= box.min.x && position.x + radius <= box.max.x
            && position.y - radius >= box.min.y && position.y + radius <= box.max.y;

        return false;
    }

    bool BoundingCircle::isPointInside(glm::vec2 point) const
    {
        return (point.x - position.x) * (point.x - position.x) + (point.y - position.y) * (point.y - position.y) <= radius * radius;
    }

} // namespace GleamHouse
