#include "Kinematic.h"

float Kinematic::getNewOrientation(float t_orientation, sf::Vector2f t_velocity)
{
	if (vectorLength(t_velocity) > 0)
	{
		return toDegrees(atan2(-t_velocity.x, t_velocity.y));
	}
	else
	{
		return t_orientation;
	}
}

float Kinematic::vectorLength(sf::Vector2f t_velocity)
{
	return sqrtf(powf(t_velocity.x, 2.0f) + powf(t_velocity.y, 2.0f));
}

float Kinematic::toDegrees(float t_rads)
{
	return t_rads * PI / 180;
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
