#include "Sweeper.h"

sweeper::sweeper(sf::Vector2f start, vector<sf::Vector2f> roomPosition, vector<sf::Vector2i> roomSize, int current, sf::Font font) :
	PI(atan(1) * 4),
	position(start),
	rotation(0),
	radian(rotation* PI / 180),
	speed(70),
	radius(150.0f),
	m_target(sf::Vector2f(rand() % roomSize[current].x + (roomPosition[current].x - roomSize[current].x / 2), rand() % roomSize[current].y + (roomPosition[current].y - roomSize[current].y / 2))),
	m_aiStates(AIStates::Wander),
	viewAngle(50),
	viewRange(300.0f),
	m_searchCounter(0),
	timer(0),
	m_roomPosition(roomPosition),
	m_roomSize(roomSize),
	m_currentRoom(current),
	m_font(font),
	m_inRoom(false)
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

	m_workerText.setFont(m_font);
	m_workerText.setFillColor(sf::Color::White);
	m_workerText.setOutlineColor(sf::Color::Black);
	m_workerText.setOutlineThickness(2);
	m_workerText.setScale(sf::Vector2f(0.5f, 0.5f));
	//m_workerText.setPosition(sf::Vector2f(position.x - 20, position.y + 50));

	m_triangle.setPointCount(3);
	m_triangle.setFillColor(sf::Color(0, 255, 0, 100));

	size = sf::Vector2f(m_texture.getSize());
}

sweeper::~sweeper() {

}

void sweeper::update(sf::Vector2f player,float t) {

	m_nextRoom = m_currentRoom + 1;
	if (m_nextRoom >= m_roomPosition.size()) {
		m_nextRoom = 0;
	}

	m_previousRoom = m_currentRoom - 1;
	if (m_previousRoom < 0) {
		m_previousRoom = m_roomPosition.size() - 1;
	}

	vision(player, 2);

	bounceOff();

	switch (m_aiStates) {
	case AIStates::Wander:
		wander(m_target, t);
		break;
	case AIStates::Seek:
		seek(m_target, t);
		break;
	case AIStates::Flee:
		flee(player, t);
		break;
	}

	radian = rotation * PI / 180;

	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);

	/*show how many worker catched */
	m_workerText.setString("Workers loaded : " + std::to_string(m_worker));
	m_workerText.setPosition(sf::Vector2f(position.x - 20, position.y + 50));

	/*set the view range position*/
	m_triangle.setPoint(0, position);

	float viewRadian = viewAngle * PI / 180;
	m_viewEdge.x = position.x + sin((-radian + PI / 2) - (viewRadian / 2)) * viewRange;
	m_viewEdge.y = position.y + cos((-radian + PI / 2) - (viewRadian / 2)) * viewRange;
	m_triangle.setPoint(1, m_viewEdge);

	m_viewEdge.x = position.x + sin((-radian + PI / 2) + (viewRadian / 2)) * viewRange;
	m_viewEdge.y = position.y + cos((-radian + PI / 2) + (viewRadian / 2)) * viewRange;
	m_triangle.setPoint(2, m_viewEdge);

	/*going back for 1 sec*/
	if (timer > 0) {
		timer -= t;
		if (timer <= 0) {
			speed = 50;
		}
	}

	m_inRoom = false;
}

void sweeper::vision(sf::Vector2f worker, int style) {
	// check the distance between player and guard
	float distance = (worker.x - position.x) * (worker.x - position.x) + (worker.y - position.y) * (worker.y - position.y);
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
		if (style == 1) {
			m_target = worker;
			m_aiStates = AIStates::Seek;
		}
		else if (style == 2 && m_aiStates != AIStates::Flee) {
			m_aiStates = AIStates::Flee;

			float distanceNext;
			distanceNext = (worker.x - m_roomPosition[m_nextRoom].x) * (worker.x - m_roomPosition[m_nextRoom].x) + (worker.y - m_roomPosition[m_nextRoom].y) + (worker.y - m_roomPosition[m_nextRoom].y);
			distanceNext = sqrt(distanceNext);

			float distancePre;
			distancePre = (worker.x - m_roomPosition[m_previousRoom].x) * (worker.x - m_roomPosition[m_previousRoom].x) + (worker.y - m_roomPosition[m_previousRoom].y) + (worker.y - m_roomPosition[m_previousRoom].y);
			distancePre = sqrt(distancePre);

			if (distanceNext >= distancePre) {
				m_target = m_roomPosition[m_nextRoom];
				m_currentRoom = m_nextRoom;
			}
			else {
				m_target = m_roomPosition[m_previousRoom];
				m_currentRoom = m_previousRoom;
			}
		}
	}
	else {
		m_triangle.setFillColor(sf::Color(0, 255, 0, 100));
	}

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
			rotation = rotation - rotate * t * 3;

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
			rotation = rotation + rotate * t * 3;

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

	/* sweeper will move the next room when it arrive to the target positoin 3 times in one room*/
	if (distance < 75.0f) {
		m_target = sf::Vector2f(rand() % m_roomSize[m_currentRoom].x + (m_roomPosition[m_currentRoom].x - m_roomSize[m_currentRoom].x / 2), rand() % m_roomSize[m_currentRoom].y + (m_roomPosition[m_currentRoom].y - m_roomSize[m_currentRoom].y / 2));
		cout << "x: " << m_target.x << " y: " << m_target.y << endl;
		m_searchCounter++;
		if (m_searchCounter > 3) {
			m_searchCounter = 0;
			m_currentRoom++;
			if (m_currentRoom >= m_roomPosition.size()) {
				m_currentRoom = 0;
			}
			m_target = m_roomPosition[m_currentRoom];
		}
	}

}

/*
the object will run away from the player position
*/
void sweeper::flee(sf::Vector2f player, float t) {
	float distanceX = m_target.x - position.x;
	float distanceY = m_target.y - position.y;
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
			rotation = rotation - rotate * t * 3;

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
			rotation = rotation + rotate * t * 3;

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

	float distance = (m_target.x - position.x) * (m_target.x - position.x) + (m_target.y - position.y) + (m_target.y - position.y);
	distance = sqrt(distance);

	if (distance < 75.0f) {
		wanderState();
	}
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
	window.draw(m_workerText);
}

sf::FloatRect sweeper::boundingBox()
{
	//bounding box for collision
	sf::FloatRect boundingBox(m_sprite.getGlobalBounds().left + 10,
		m_sprite.getGlobalBounds().top + 10,
		m_sprite.getGlobalBounds().width - 10,
		m_sprite.getGlobalBounds().height - 10);
	return boundingBox;
}

void sweeper::wanderState() {
	m_target = sf::Vector2f(rand() % m_roomSize[m_currentRoom].x + (m_roomPosition[m_currentRoom].x - m_roomSize[m_currentRoom].x / 2), rand() % m_roomSize[m_currentRoom].y + (m_roomPosition[m_currentRoom].y - m_roomSize[m_currentRoom].y / 2));
	cout << "wander! x: " << m_target.x << " y: " << m_target.y << endl;
	m_aiStates = AIStates::Wander; 
}

void sweeper::bounceOff() {
	if (!m_inRoom) {
		speed = -speed;
		rotation += 90;
		timer = 1.0f;

		m_inRoom = true;
	}
}
