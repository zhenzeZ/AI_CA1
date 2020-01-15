#include "Sweeper.h"

sweeper::sweeper(sf::Vector2f start, vector<sf::Vector2f> roomPosition, int current) :
	PI(atan(1) * 4),
	position(start),
	rotation(0),
	radian(rotation* PI / 180),
	speed(150),
	radius(150.0f),
	m_target(sf::Vector2f(rand() % 800, rand() % 600)),
	m_aiStates(AIStates::Wander),
	viewAngle(50),
	viewRange(150.0f),
	m_currentRoom(current)
{

	if (!m_texture.loadFromFile("./ASSETS/IMAGES/Sweeper.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check

	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);
	m_sprite.setOrigin(sf::Vector2f(m_texture.getSize().x / 2, m_texture.getSize().y / 2));

	m_triangle.setPointCount(3);
	m_triangle.setFillColor(sf::Color(255, 255, 255, 100));
}

sweeper::~sweeper() {

}

void sweeper::update(sf::Vector2f player,sf::Vector2f worker, float t) {

	m_nextRoom = m_currentRoom++;
	if (m_nextRoom >= m_roomPosition.size()) {
		m_nextRoom = 0;
	}

	previousRoom = m_currentRoom--;
	if (previousRoom < 0) {
		previousRoom = m_roomPosition.size() - 1;
	}


	switch (m_aiStates) {
	case AIStates::Wander:
		wander(m_target, t);
		break;
	case AIStates::Seek:
		seek(player, t);
		break;
	case AIStates::Flee:
		flee(player, t);
		break;
	}

	radian = rotation * PI / 180;

	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);

	vision(player);

}

void sweeper::vision(sf::Vector2f worker) {
	// check the distance between player and guard
	float distance = (worker.x - position.x) * (worker.x - position.x) + (worker.y - position.y) + (worker.y - position.y);
	distance = sqrt(distance);

	sf::Vector2f facing = velocity;
	sf::Vector2f target = worker - position;

	float facingLength = sqrtf(facing.x * facing.x + facing.y * facing.y);
	float targetLength = sqrtf(target.x * target.x + target.y * target.y);

	float scalar_product = (facing.x * target.x + facing.y * target.y) / (facingLength * targetLength);

	float radianBetween = acos(scalar_product);
	float angleBetWeen = radianBetween * 180 / PI;

	//std::cout << angleBetWeen << std::endl;


	if (viewAngle / 2 > angleBetWeen && distance < viewRange) {
		m_triangle.setFillColor(sf::Color(255, 0, 0, 100));
		m_aiStates = AIStates::Seek;
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
void sweeper::wander(sf::Vector2f target, float t) {
	float distanceX = target.x - position.x;
	float distanceY = target.y - position.y;
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


	float distance = (target.x - position.x) * (target.x - position.x) + (target.y - position.y) + (target.y - position.y);
	distance = sqrt(distance);
	if (distance < 50.0f) {
		m_target = sf::Vector2f(rand() % 800, rand() % 600);
	}
}

/*
the object will run away from the player position
*/
void sweeper::flee(sf::Vector2f player, float t) {

	float distanceNext;
	distanceNext = (player.x - m_roomPosition[m_nextRoom].x) * (player.x - m_roomPosition[m_nextRoom].x) + (player.y - m_roomPosition[m_nextRoom].y) + (player.y - m_roomPosition[m_nextRoom].y);
	distanceNext = sqrt(distanceNext);

	float distancePre;
	distancePre = (player.x - m_roomPosition[previousRoom].x) * (player.x - m_roomPosition[previousRoom].x) + (player.y - m_roomPosition[previousRoom].y) + (player.y - m_roomPosition[previousRoom].y);
	distancePre = sqrt(distancePre);

	if (distanceNext >= distancePre) {
		m_target = m_roomPosition[m_nextRoom];
	}
	else {
		m_target = m_roomPosition[previousRoom];
	}

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


}

/*
the object will follow the player position
the player position could be pursued
*/
void sweeper::seek(sf::Vector2f worker, float t)
{
	float distanceX = worker.x - position.x;
	float distanceY = worker.y - position.y;
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
}
