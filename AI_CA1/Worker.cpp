#include "Worker.h"

worker::worker(sf::Vector2f start) :
	PI(atan(1) * 4),
	position(start),
	rotation(0),
	radian(rotation* PI / 180),
	speed(150),
	radius(150.0f),
	m_target(sf::Vector2f(rand() % 800, rand() % 600)),
	viewAngle(50),
	viewRange(150.0f)
{

	if (!m_texture.loadFromFile("./ASSETS/IMAGES/worker.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check

	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);
	m_sprite.setOrigin(sf::Vector2f(m_texture.getSize().x / 2, m_texture.getSize().y / 2));

}

worker::~worker() {

}

void worker::update(float t) {

	wander(m_target, t);

	radian = rotation * PI / 180;

	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);

}

/*
the object will follow the random target position
*/
void worker::wander(sf::Vector2f player, float t) {
	float distanceX = player.x - position.x;
	float distanceY = player.y - position.y;
	float x = 0;
	float targetRadian = atan(x);

	if (distanceY != 0) {
		targetRadian = atan2(distanceY, distanceX);
	}


	float targetRotation = targetRadian * 180 / PI;

	if (rotation < targetRotation)
	{
		if ((targetRotation - rotation) > (rotation + 360 - targetRotation))
		{
			float rotate = rotation - (targetRotation - 360);
			rotation = rotation - rotate * t;

			if (rotation < -180) {
				rotation = 180;
			}
		}
		else {

			float rotate = targetRotation - rotation;
			rotation = rotation + rotate * t;
		}


	}

	if (rotation > targetRotation)
	{
		if ((rotation - targetRotation) > (targetRotation + 360 - rotation))
		{
			float rotate = (targetRotation + 360) - rotation;
			rotation = rotation + rotate * t;

			if (rotation > 180) {
				rotation = -180;
			}
		}
		else {

			float rotate = rotation - targetRotation;
			rotation = rotation - rotate * t;
		}


	}

	velocity.x = speed * cos(radian);
	velocity.y = speed * sin(radian);

	position.x += velocity.x * t;
	position.y += velocity.y * t;


	float distance = (player.x - position.x) * (player.x - position.x) + (player.y - position.y) + (player.y - position.y);
	distance = sqrt(distance);
	if (distance < 50.0f) {
		m_target = sf::Vector2f(rand() % 800, rand() % 600);
	}
}

/*
the player catch the worker when the distance less then 10
*/
bool worker::catchCheck(sf::Vector2f player) {
	float distance = (player.x - position.x) * (player.x - position.x) + (player.y - position.y) + (player.y - position.y);
	distance = sqrt(distance);

	if (distance < 10.0f) {
		return true;
	}
	return false;
}

void worker::render(sf::RenderWindow& window) {
	window.draw(m_sprite);
}
