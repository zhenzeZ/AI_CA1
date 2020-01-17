/// <summary>
/// @author Dion and Zhen
/// @date Jan 2020
///
/// </summary>
#include "Game.h"
#include <iostream>


/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1000U, 1000U, 32U }, "CA1" },
	m_exitGame{ false } //when true game will exit
{
	m_window.setVerticalSyncEnabled(true);
	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	setupMap(); // create map
	setupWorkers(); // spawn workers
	setUpSweeper(); // spawn sweeper

	m_player = new player(sf::Vector2f(600, 300), m_font);
	m_alienNest = new AlienNest(*m_player, sf::Vector2f{300, -400});
	m_alienNest2 = new AlienNest(*m_player, sf::Vector2f{ 400, 750 });

	gameView.setViewport(sf::FloatRect(0, 0, 1, 1)); // fullscreen
	gameView.setCenter(m_player->playerPosition()); // set mid of screen to camera
	minimapView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f)); 
	m_window.setView(gameView);

	m_itemSpawnRate = 10.0f;
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// 
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{

}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps

			if (m_player->getHealth() >= 0) {
				update(timePerFrame); //60 fps
			}
		}
		render(); // as many as possible
	}
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	if (m_itemSpawnRate <= 0.0f) {
		m_itemSpawnRate = 10.0f;
		spawnItem();
	}
	else {
		m_itemSpawnRate -= t_deltaTime.asSeconds();
	}

	m_alienNest->update(t_deltaTime.asSeconds());
	m_alienNest2->update(t_deltaTime.asSeconds());

	gameView.setCenter(m_player->playerPosition()); // set mid of screen to camera
	minimapView.setCenter(m_player->playerPosition()); // set mid of screen to camera

	/*fire a bullet and reduce the ammo value in player class*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
		m_player->m_fireRate <= 0 &&
		m_player->getAmmo() > 0)
	{
		// fire a bullet

		m_bullets.push_back(new bullet(m_player->playerPosition(), m_player->playerRadian()));
		m_player->m_fireRate = 0.2f;
		m_player->ammoChanged(-1);
	}

	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i]->update(t_deltaTime.asSeconds());
		for (int x = 0; x < m_sweepers.size(); x++) {
			if (m_bullets[i]->collisionCheck(m_sweepers[x]->boundingBox())) {
				m_player->saveWorker(m_sweepers[x]->getWorker());
				m_sweepers.erase(m_sweepers.begin() + x);
				m_bullets[i]->dead();
			}
		}
		if (!m_bullets[i]->checkAlive()) {
			m_bullets.erase(m_bullets.begin() + i);
			break;
		}
		if (m_bullets[i]->boundingBox().intersects(m_alienNest->boundingBox())) {
			m_alienNest->damage();
			m_bullets.erase(m_bullets.begin() + i);
			break;
		}
		else if (m_bullets[i]->boundingBox().intersects(m_alienNest2->boundingBox())) {
			m_alienNest2->damage();
			m_bullets.erase(m_bullets.begin() + i);
			break;
		}
	}

	/*get a random power up effect when player get a item box*/
	for (int i = 0; i < m_items.size(); i++) {
		if (m_items[i]->collisionCheck(m_player->boundingBox())) {
			m_player->powerUps(rand() % 3 + 1);
			m_items.erase(m_items.begin() + i);
		}
	}

	/* player movement*/
	m_player->update(t_deltaTime.asSeconds());

	if (m_player->boundingBox().intersects(m_alienNest->missileBoundingBox())) {
		m_player->damage();
		m_alienNest->destroyMissile();
	}
	if (m_player->boundingBox().intersects(m_alienNest2->missileBoundingBox())) {
		m_player->damage();
		m_alienNest2->destroyMissile();
	}

	for (int i = 0; i < m_rooms.size(); i++) {
		if (m_rooms[i]->isPlayerInRoom(m_player->playerSize(), m_player->playerPosition())) {
			m_player->playerInTheRoom();
		}

		for (int x = 0; x < m_workers.size(); x++) {
			if (m_rooms[i]->isPlayerInRoom(m_workers[x]->workerSize(), m_workers[x]->workerPosition())) {
				m_workers[x]->inTheRoom();
			}
		}

		for (int x = 0; x < m_sweepers.size(); x++) {
			if (m_rooms[i]->isPlayerInRoom(m_sweepers[x]->sweeperSize(), m_sweepers[x]->sweeperPosition())) {
				m_sweepers[x]->inTheRoom();
			}
		}
	}

	/* worker movement*/
	for (int i = 0; i < m_workers.size(); i++) {
		m_workers[i]->update(t_deltaTime.asSeconds());
		if (m_workers[i]->collisionCheck(m_player->boundingBox())) {
			m_player->saveWorker(1);
			m_workers.erase(m_workers.begin() + i);
			break;
		}

		for (int x = 0; x < m_sweepers.size(); x++) {
			m_sweepers[x]->vision(m_workers[i]->workerPosition(),1);
			if (m_workers[i]->collisionCheck(m_sweepers[x]->boundingBox())) {
				m_workers.erase(m_workers.begin() + i);
				m_sweepers[x]->catchWorker();
				m_sweepers[x]->wanderState();
				break;
			}
		}
	}

	/*sweeper movement*/
	for (int i = 0; i < m_sweepers.size(); i++) {
		m_sweepers[i]->update(m_player->playerPosition(), t_deltaTime.asSeconds());

	}

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);

	for (int i = 0; i < m_rooms.size(); i++) {
		m_rooms[i]->render(m_window);
	}

	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i]->render(m_window);

	}

	for (int i = 0; i < m_workers.size(); i++) {
		m_workers[i]->render(m_window);
	}

	for (int i = 0; i < m_sweepers.size(); i++) {
		m_sweepers[i]->render(m_window);
	}

	for (int i = 0; i < m_items.size(); i++) {
		m_items[i]->render(m_window);
	}

	m_player->render(m_window);
	m_alienNest->render(m_window);
	m_alienNest2->render(m_window);

	m_window.setView(minimapView);
	sf::RectangleShape border(minimapView.getSize());
	border.setFillColor(sf::Color::Yellow); // temp colour
	border.setOrigin(sf::Vector2f{ border.getSize().x / 2, border.getSize().y / 2 });
	border.setPosition(m_player->playerPosition());
	m_window.draw(border);
	sf::RectangleShape walls;
	walls.setFillColor(sf::Color::Black);
	for (int i = 0; i < m_rooms.size(); i++) {
		walls.setPosition(m_rooms[i]->getPosition());
		walls.setSize(sf::Vector2f(m_rooms[i]->getSize()));
		m_window.draw(walls);
	}

	sf::CircleShape object;
	object.setRadius(20);
	object.setFillColor(sf::Color::Blue);
	object.setPosition(m_player->playerPosition());
	m_window.draw(object);
	object.setFillColor(sf::Color::Red);
	//for (auto& e : enemies)
	//{	
	//}
	object.setPosition(m_alienNest->getPosition());
	m_window.draw(object);
	object.setPosition(m_alienNest2->getPosition());
	m_window.draw(object);

	m_window.setView(gameView);

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupMap()
{
	m_numOfRoom = 0;

	m_roomPosition.push_back(sf::Vector2f(500, 500));
	m_roomSize.push_back(sf::Vector2i(600, 600));

	m_roomPosition.push_back(sf::Vector2f(500, 0));
	m_roomSize.push_back(sf::Vector2i(300, 550));

	m_roomPosition.push_back(sf::Vector2f(500, -700));
	m_roomSize.push_back(sf::Vector2i(1000, 1000));

	m_roomPosition.push_back(sf::Vector2f(-400, -700));
	m_roomSize.push_back(sf::Vector2i(950, 300));

	m_roomPosition.push_back(sf::Vector2f(-1100, -700));
	m_roomSize.push_back(sf::Vector2i(600, 600));

	m_roomPosition.push_back(sf::Vector2f(-1100, -200));
	m_roomSize.push_back(sf::Vector2i(300, 550));

	m_roomPosition.push_back(sf::Vector2f(-1100, 500));
	m_roomSize.push_back(sf::Vector2i(1000, 1000));

	m_roomPosition.push_back(sf::Vector2f(-200, 500));
	m_roomSize.push_back(sf::Vector2i(950, 300));
	

	for (int i = 0; i < m_roomPosition.size(); i++) {
		m_room = new room(sf::Vector2f(m_roomSize[i]), m_roomPosition[i]); // size and position
		m_rooms.push_back(m_room);
		m_numOfRoom++;
	}

	cout << m_numOfRoom << endl;
}

/// <summary>
/// randomly spawn the worker on the map
/// </summary>
void Game::setupWorkers()
{
	m_maxWorker = 15;

	for (int i = 0; i < m_maxWorker; i++) {

		int spawnRoom = std::rand() % m_numOfRoom;
		//int spawnRoom = 1;
		m_worker = new worker(m_rooms[spawnRoom]->getOrigin(), m_rooms[spawnRoom]->getPosition(), m_rooms[spawnRoom]->getSize());
		m_workers.push_back(m_worker);
	}
}

/// <summary>
/// spawn the Sweeper
/// </summary>
void Game::setUpSweeper() {
	m_maxSweeper = 5;

	for (int i = 0; i < m_maxSweeper; i++) {
		int spawnRoom = std::rand() % m_numOfRoom;
		//int spawnRoom = 0;
		m_sweeper = new sweeper(m_roomPosition[spawnRoom], m_roomPosition, m_roomSize , spawnRoom, m_font);
		m_sweepers.push_back(m_sweeper);
	}
}

void Game::spawnItem() {

	if (m_items.size() < 5) {
		int spawnRoom = std::rand() % m_numOfRoom;
		m_item = new item(m_rooms[spawnRoom]->getPosition(), m_rooms[spawnRoom]->getSize());
		m_items.push_back(m_item);
	}
}
