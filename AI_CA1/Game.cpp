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

	setupMap(); // load font 
	setupWorkers(); // load texture

	m_player = new player(sf::Vector2f(600, 500));
	m_alienNest = new AlienNest(*m_player);
	m_alienNest2 = new AlienNest(*m_player);

	gameView.setViewport(sf::FloatRect(0, 0, 1, 1)); // fullscreen
	gameView.setCenter(m_player->playerPosition()); // set mid of screen to camera
	minimapView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f)); 
	m_window.setView(gameView);
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
			update(timePerFrame); //60 fps
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

	gameView.setCenter(m_player->playerPosition()); // set mid of screen to camera


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		// fire a bullet
		m_bullets.push_back(new bullet(m_player->playerPosition(), m_player->playerRadian()));
	}

	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i]->update(t_deltaTime.asSeconds());

		if (!m_bullets[i]->checkAlive()) {
			m_bullets.erase(m_bullets.begin() + i);
		}
	}

	m_player->update(t_deltaTime.asSeconds());

	for (int i = 0; i < m_workers.size(); i++) {
		m_workers[i]->update(t_deltaTime.asSeconds());
		if (m_workers[i]->catchCheck(m_player->playerPosition())) {
			m_player->saveWorker();
			m_workers.erase(m_workers.begin() + i);
		}
	}

	/*if (m_room->isPlayerInRoom(m_player->playerSize(), m_player->playerPosition())) {
		m_player->buttonCheck();
	}*/

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

	m_player->render(m_window);

	m_window.setView(minimapView);
	sf::RectangleShape border(minimapView.getSize());
	border.setFillColor(sf::Color::Red);
	m_window.draw(border);
	sf::CircleShape object;
	object.setRadius(20);
	object.setFillColor(sf::Color::Blue);
	object.setPosition(m_player->playerPosition());
	m_window.draw(object);
	object.setFillColor(sf::Color::Red);
	//for (auto& e : enemies)
	{
		//object.setPosition(e->getPosition());
		//window.draw(object);
	}

	m_window.setView(gameView);

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupMap()
{
	m_room = new room(sf::Vector2f(600, 600), sf::Vector2f(500, 500)); // size and position
	m_rooms.push_back(m_room);

	m_room = new room(sf::Vector2f(200, 400), sf::Vector2f(500, 0)); // size and position
	m_rooms.push_back(m_room);

	m_room = new room(sf::Vector2f(1000, 1000), sf::Vector2f(500, -700)); // size and position
	m_rooms.push_back(m_room);
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupWorkers()
{
	m_worker = new worker(sf::Vector2f(300, 300));
	m_workers.push_back(m_worker);
}
