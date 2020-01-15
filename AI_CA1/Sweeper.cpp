#include "Sweeper.h"

sweeper::sweeper(sf::Vector2f start, AIStates state) :
	PI(atan(1) * 4),
	position(start),
	rotation(0),
	radian(rotation* PI / 180),
	speed(150),
	radius(150.0f),
	m_target(sf::Vector2f(rand() % 800, rand() % 600)),
	m_aiStates(state),
	viewAngle(50),
	viewRange(150.0f)
{

	if (!m_texture.loadFromFile("./ASSETS/IMAGES/object1.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check

	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_behaviour.setFont(m_font);
	m_behaviour.setCharacterSize(20);
	//m_behaviour.setOutlineColor(sf::Color::Red);
	m_behaviour.setFillColor(sf::Color::White);
	m_behaviour.setOutlineThickness(0.0f);

	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);
	m_sprite.setOrigin(sf::Vector2f(m_texture.getSize().x / 2, m_texture.getSize().y / 2));

	m_triangle.setPointCount(3);
	m_triangle.setFillColor(sf::Color(255, 255, 255, 100));
}

sweeper::~sweeper() {

}

void sweeper::update(sf::Vector2f player, float t) {

	switch (m_aiStates) {
	case AIStates::Wander:
		wander(m_target, t);
		m_behaviour.setString("Wander");
		break;
	case AIStates::Seek:
		seek(player, t);
		m_behaviour.setString("Seek");
		break;
	case AIStates::Flee:
		flee(player, t);
		m_behaviour.setString("Flee");
		break;
	}

	m_behaviour.setPosition(position);

	radian = rotation * PI / 180;

	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);

	vision(player);

}

void sweeper::vision(sf::Vector2f player) {
	// check the distance between player and guard
	float distance = (player.x - position.x) * (player.x - position.x) + (player.y - position.y) + (player.y - position.y);
	distance = sqrt(distance);

	sf::Vector2f facing = velocity;
	sf::Vector2f target = player - position;

	float facingLength = sqrtf(facing.x * facing.x + facing.y * facing.y);
	float targetLength = sqrtf(target.x * target.x + target.y * target.y);

	float scalar_product = (facing.x * target.x + facing.y * target.y) / (facingLength * targetLength);

	float radianBetween = acos(scalar_product);
	float angleBetWeen = radianBetween * 180 / PI;

	//std::cout << angleBetWeen << std::endl;


	if (viewAngle / 2 > angleBetWeen && distance < viewRange) {
		m_triangle.setFillColor(sf::Color(255, 0, 0, 100));
	}
	else {
		m_triangle.setFillColor(sf::Color(255, 255, 255, 100));
	}

	m_triangle.setPoint(0, position);

	float viewRadian = viewAngle * PI / 180;
	m_viewEdge.x = position.x + sin((-radian + PI / 2) - (viewRadian / 2)) * viewRange;
	m_viewEdge.y = position.y + cos((-radian + PI / 2) - (viewRadian / 2)) * viewRange;
	m_triangle.setPoint(1, m_viewEdge);

	m_viewEdge.x = position.x + sin((-radian + PI / 2) + (viewRadian / 2)) * viewRange;
	m_viewEdge.y = position.y + cos((-radian + PI / 2) + (viewRadian / 2)) * viewRange;
	m_triangle.setPoint(2, m_viewEdge);
}

/*
the object will follow the random target position
*/
void sweeper::wander(sf::Vector2f player, float t) {
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
the object will run away from the player position
*/
void sweeper::flee(sf::Vector2f player, float t) {
	float distanceX = player.x - position.x;
	float distanceY = player.y - position.y;
	float x = 0;
	float targetRadian = atan(x);

	if (distanceY != 0) {
		targetRadian = atan2(distanceY, distanceX);
	}

	float targetRotation = targetRadian * 180 / PI;
	targetRotation += 180;

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
}

/*
the object will follow the player position
the player position could be pursued
*/
void sweeper::seek(sf::Vector2f player, float t)
{
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
		// the target rotation between -180 and 180 if the rotation from the 180 to -179 and the target rotation will change to 181
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
		// the target rotation between -180 and 180 if the rotation from the -180 to 179 and the target rotation will change to -181
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
}


void sweeper::render(sf::RenderWindow& window) {
	window.draw(m_sprite);
	window.draw(m_triangle);
	window.draw(m_behaviour);
}
