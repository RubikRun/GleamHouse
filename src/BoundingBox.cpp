#include "BoundingBox.h"

#include "BoundingCircle.h"

namespace GleamHouse
{

	bool BoundingBox::collides(BoundingBox other) const
    {
        return max.x >= other.min.x && min.x <= other.max.x && max.y >= other.min.y && min.y <= other.max.y;
    }

    bool BoundingBox::collides(const BoundingCircle& circle) const
    {
        // Clamp circle's position to box's range
        // to find the point inside the box that is closest to the circle.
        const glm::vec2 closest = glm::clamp(circle.position, min, max);

        // If the distance between circle's center and the closest point
        // is less than, or equal to, circle's radius then we have a collision
        const glm::vec2 circleToClosest = circle.position - closest;
        return (circleToClosest.x * circleToClosest.x + circleToClosest.y * circleToClosest.y) <= (circle.radius * circle.radius);
    }

    bool BoundingBox::isPointInside(glm::vec2 point) const
    {
        return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
    }

} // namespace GleamHouse
