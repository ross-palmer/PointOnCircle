#include "MathUtility.h"

namespace MathUtility
{
	////////////////////////////////////////////////////////////
	double distance(sf::Vector2f v1, sf::Vector2f v2)
	{
		return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
	}

	////////////////////////////////////////////////////////////
	bool lineIntersectsCircle(sf::Vector2f ahead, sf::Vector2f halfAhead, sf::CircleShape circle)
	{
		return distance(circle.getPosition(), ahead) <= circle.getRadius() ||
			distance(circle.getPosition(), halfAhead) <= circle.getRadius();
	}

	////////////////////////////////////////////////////////////
	sf::Vector2f truncate(sf::Vector2f v, float const max)
	{		
		if (thor::length(v) > max)
		{
			v = thor::unitVector(v);			
			v *= max;
		}	

		return v;
	}

	////////////////////////////////////////////////////////////
	float checkProjection(sf::Vector2f t_v1, sf::Vector2f t_v2)
	{
		t_v1 = thor::unitVector(t_v1);
		t_v2 = thor::unitVector(t_v2);
		return thor::dotProduct(t_v1, t_v2);
	}

	bool inFieldOfView(float t_fieldOfView, sf::Vector2f t_dirFacing, sf::Vector2f t_dirToTarget)
	{		
		t_dirToTarget = thor::unitVector(t_dirToTarget);
		// θ = acos(AB)
		float angle = thor::toDegree(std::acos(t_dirFacing.x * t_dirToTarget.x + t_dirFacing.y * t_dirToTarget.y));
		if (angle < (t_fieldOfView / 2.0f))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
