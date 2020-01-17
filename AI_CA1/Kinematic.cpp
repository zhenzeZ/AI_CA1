#include "Kinematic.h"

float Kinematic::getNewOrientation(sf::Vector2f t_position1, sf::Vector2f t_position2)
{
	return toDegrees(atan2(t_position2.y - t_position1.y, t_position2.x - t_position1.x));
}

float Kinematic::vectorLength(sf::Vector2f t_velocity)
{
	return sqrtf(powf(t_velocity.x, 2.0f) + powf(t_velocity.y, 2.0f));
}

float Kinematic::toDegrees(float t_rads)
{
	return t_rads * 180 / PI;
}

void Kinematic::screenWrap(sf::Vector2f& t_position)
{
	
	if (t_position.x < 0 - SPRITE_QUARTER)
	{
		t_position = sf::Vector2f(SCR_W + SPRITE_QUARTER, t_position.y);
	}
	if (t_position.x > SCR_W + SPRITE_QUARTER)
	{
		t_position = sf::Vector2f(0 - SPRITE_QUARTER, t_position.y);
	}
	if (t_position.y < 0 - SPRITE_QUARTER)
	{
		t_position = sf::Vector2f(t_position.x, SCR_H + SPRITE_QUARTER);
	}
	if (t_position.y > SCR_H + SPRITE_QUARTER)
	{
		t_position = sf::Vector2f(t_position.x, 0 - SPRITE_QUARTER);
	}
}
