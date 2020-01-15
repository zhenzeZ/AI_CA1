#include "Player.h"

player::player(sf::Vector2f start) :
	PI(acos(-1)),
	acceleration(0),
	accelerating(false),
	maxSpeed(300),
	rotation(0),
	radian(rotation* PI / 180),
	position(start),
	speed(0),
	pursueTime(0.5),
	m_playerInRoom(false)
{

	if (!m_texture.loadFromFile("./ASSETS/IMAGES/player.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check

	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);
	m_sprite.setOrigin(sf::Vector2f(m_texture.getSize().x / 2, m_texture.getSize().y / 2));

	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	m_workers.setFont(m_font);
	m_workers.setCharacterSize(20);
	m_workers.setFillColor(sf::Color::White);
	m_workers.setOutlineThickness(0.0f);

	size = sf::Vector2f(m_texture.getSize());

	m_view.setCenter(position); // set mid of screen to camera
	m_view.setSize(sf::Vector2f(1000, 1000)); // set camera's size
}

player::~player() {

}

void player::update(float t) {

	bounceOff(); // when player running out the room

	buttonCheck();

	movementCalculate(t);

	if (!accelerating) {
		speed *= 0.98;
		if (speed < 0.5f) {
			speed = 0;
		}
	}

	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);

	m_workers.setPosition(position);

	m_playerInRoom = false;
}

void player::movementCalculate(float t) {
	radian = rotation * PI / 180;

	speed = speed + acceleration * t;
	if (speed > maxSpeed)
	{
		speed = maxSpeed;
	}
	else if (speed < -maxSpeed)
	{
		speed = -maxSpeed;
	}

	velocity.x = speed * cos(radian);
	velocity.y = speed * sin(radian);

	position.x += velocity.x * t;
	position.y += velocity.y * t;

	pursue = position + velocity * pursueTime;

	m_view.setCenter(position); // set mid of screen to camera
}

void player::bounceOff() {
	if (!m_playerInRoom) {
		speed = -speed * 0.5f;

		m_playerInRoom = true;
	}
}

/*
turn around, speed up and speed down with button pressed
*/
void player::buttonCheck() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		acceleration = 150.0f;
		accelerating = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		acceleration = -150.0f;
		accelerating = true;
	}
	else {
		acceleration = 0.0f;
		accelerating = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		rotation -= 2;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		rotation += 2;
	}
}

void player::render(sf::RenderWindow& window) {

	window.setView(m_view);

	window.draw(m_sprite);
}

sf::FloatRect player::boundingBox()
{
	//bounding box for collision
	sf::FloatRect boundingBox(m_sprite.getGlobalBounds().left + 10,
		m_sprite.getGlobalBounds().top + 10,
		m_sprite.getGlobalBounds().width - 10,
		m_sprite.getGlobalBounds().height - 10);
	return boundingBox;
}