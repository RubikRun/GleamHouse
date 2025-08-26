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

} // namespace GleamHouse
