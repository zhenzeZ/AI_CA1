#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
// base level consts, consider moving to own file
static const float SPRITE_QUARTER = 142.0f / 4.0f;
static const float MAX_VELOCITY = 3.0f;
static const float MIN_VELOCITY = 0.05f;
static const float ROTATION_CHANGE = 6.0f;
static const unsigned int SCR_W = sf::VideoMode::getDesktopMode().width;
static const unsigned int SCR_H = sf::VideoMode::getDesktopMode().height;
static const float PI = 3.14159265359;
static const float ONE_RAD = 180 / PI;
class Kinematic 
{
public:
	float static getNewOrientation(sf::Vector2f t_position1, sf::Vector2f t_position2);
	float static vectorLength(sf::Vector2f t_velocity);
	float static toDegrees(float t_rads);
	void static screenWrap(sf::Vector2f& t_position);
};