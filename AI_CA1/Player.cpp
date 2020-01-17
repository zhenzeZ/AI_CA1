#include "Player.h"

player::player(sf::Vector2f start, sf::Font font) :
	PI(acos(-1)),
	m_font(font),
	acceleration(0),
	accelerating(false),
	maxSpeed(300),
	rotation(0),
	radian(rotation* PI / 180),
	position(start),
	speed(0),
	pursueTime(0.5),
	m_fireRate(0.0f),
	m_playerInRoom(false),
	m_workerSaved(0),
	m_ammo(20),
	m_health(100.0f),
	m_energy(50.0f)
{

	setUpShape();
	setUpText();

	if (!m_texture.loadFromFile("./ASSETS/IMAGES/player.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check


	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);
	m_sprite.setOrigin(sf::Vector2f(m_texture.getSize().x / 2, m_texture.getSize().y / 2));

	if (!m_barrierTexture.loadFromFile("./ASSETS/IMAGES/barrier.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check

	m_barrier.setTexture(m_barrierTexture);
	m_barrier.setPosition(position);
	m_barrier.setRotation(rotation);
	m_barrier.setOrigin(sf::Vector2f(m_barrierTexture.getSize().x / 2, m_barrierTexture.getSize().y / 2));

	m_barrierOn = false;
	m_barrierTimer = 0.0f;

	size = sf::Vector2f(m_texture.getSize());

}

player::~player() {

}

void player::setUpShape()
{
	for (int i = 0; i < 2; i++)
	{
		m_HealthBar[i].setSize(sf::Vector2f(100, 20));
		//m_HealthBar[i].setPosition(sf::Vector2f(position.x - 500, position.y - 600));
	}
	m_HealthBar[1].setFillColor(sf::Color::Red);
	m_HealthBar[0].setOutlineThickness(3);
	m_HealthBar[0].setOutlineColor(sf::Color::Yellow);
}
void player::setUpText() {
	m_ammoText.setFont(m_font);
	//m_ammoText.setFillColor(sf::Color::Blue);
	m_ammoText.setOutlineColor(sf::Color::White);
	m_ammoText.setOutlineThickness(2);
	//m_ammoText.setPosition(sf::Vector2f(position.x - 500, position.y - 500));

	m_workersText.setFont(m_font);
	m_workersText.setFillColor(sf::Color::Black);
	m_workersText.setOutlineColor(sf::Color::White);
	m_workersText.setOutlineThickness(2);
	//m_workersText.setPosition(sf::Vector2f(position.x - 500, position.y - 400));

	m_gameoverText.setFont(m_font);
	m_gameoverText.setFillColor(sf::Color::Black);
	m_gameoverText.setOutlineColor(sf::Color::Red);
	m_gameoverText.setOutlineThickness(2);
	m_gameoverText.setScale(sf::Vector2f(2.0f, 2.0f));
}

void player::update(float t) {

	bounceOff(); // when player running out the room

	buttonCheck();

	movementCalculate(t);

	if (m_fireRate > 0) {
		m_fireRate -= t;
	}

	/* timer for active barrier*/
	if (m_barrierOn) {
		m_barrierTimer -= t;
		if (m_barrierTimer <= 0) {
			m_barrierOn = false;
			m_barrierTimer = 0;
		}
	}

	if (!accelerating) {
		speed *= 0.98;
		if (speed < 0.5f) {
			speed = 0;
		}
	}

	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);

	if (m_ammo <= 5)
	{
		m_ammoText.setFillColor(sf::Color::Red);
	}
	else
	{
		m_ammoText.setFillColor(sf::Color::Black);
	}

	if (m_ammo < 0) {
		m_ammo = 0;
	}

	if (m_health <= 0) {
		m_health = 0;
	}

	m_workersText.setString("Worker saved: " + std::to_string(m_workerSaved));
	m_ammoText.setString("AMMO: " + std::to_string(m_ammo));

	m_workersText.setPosition(sf::Vector2f(position.x - 450, position.y - 450));
	m_ammoText.setPosition(sf::Vector2f(position.x - 450, position.y - 420));

	/*set health bar position and value*/
	for (int i = 0; i < 2; i++)
	{
		m_HealthBar[i].setPosition(sf::Vector2f(position.x - 450, position.y - 480));
	}
	m_HealthBar[1].setScale(sf::Vector2f(m_health / 100, 1));

	m_barrier.setPosition(position);
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
}

void player::damage()
{
	if (!m_barrierOn) {
		m_health-= 10;
	}
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

/// <summary>
/// restore health and ammo to player
/// or active the barrier for player, and player won't take damage when barrier is on
/// </summary>
/// <param name="itemStyle"></param>
void player::powerUps(int itemStyle) {
	switch (itemStyle)
	{
	case 1:
		m_ammo += 10;
		break;
	case 2:
		m_health += 20;
		break;
	case 3:
		m_barrierOn = true;
		m_barrierTimer = 7.5f;
		break;
	default:
		break;
	}
}

void player::render(sf::RenderWindow& window) {

	window.draw(m_sprite);

	for (int i = 0; i < 2; i++)
	{
		window.draw(m_HealthBar[i]);
	}

	if (m_barrierOn) {
		window.draw(m_barrier);
	}

	window.draw(m_ammoText);
	window.draw(m_workersText);

	if (m_health <= 0) {
		m_gameoverText.setPosition(position);
		window.draw(m_gameoverText);
	}

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

