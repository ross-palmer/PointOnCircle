﻿#include "Game.h"
#include <iostream>

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default)
{
	m_window.setVerticalSyncEnabled(true);

	//loads in the sprite sheet	
	if (!m_spriteSheetTexture.loadFromFile("./resources/assets/graphics/SpriteSheet.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_timingBarTexture.loadFromFile("./resources/assets/graphics/TimingBar.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	// Initialise the particle system
	m_particleSystem.initParticleSystem();

	initTankSprites();


	m_circleShape.setRadius(5);
	m_circleShape.setFillColor(sf::Color::Red);

	m_rectShape.setSize(sf::Vector2f(TIMING_BAR_WIDTH, 20));	
	m_rectShape.setPosition(600, 30);
	m_rectShape.setTexture(&m_timingBarTexture);
	// Setup the timer to run for 1 second.
	m_timer.reset(sf::Time(sf::milliseconds(TIMER_DURATION)));

	// Setup the vision cone with 30 degrees field of view.
	setVisionCone(30.0f);

	m_circleShape.setPosition(500, 300);
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();

		processEvents();

		while (lag > MS_PER_UPDATE)
		{
			update(lag);
			lag -= MS_PER_UPDATE;
		}
		update(lag);

		render();
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

////////////////////////////////////////////////////////////
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	if (sf::Event::KeyPressed == event.type)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			m_window.close();
			break;
		case sf::Keyboard::W:
			m_circleShape.move(0, -1);
			break;
		case sf::Keyboard::A:
			m_circleShape.move(-1, 0);
			break;
		case sf::Keyboard::S:
			m_circleShape.move(0, 1);
			break;
		case sf::Keyboard::R:
			m_circleShape.setPosition(500, 300);
			m_timer.reset(sf::Time(sf::milliseconds(TIMER_DURATION)));
			break;
		case sf::Keyboard::D:
			m_circleShape.move(1, 0);
			break;
		case sf::Keyboard::Right:
			m_turretSprite.rotate(1.0f);
			break;
		case sf::Keyboard::Left:
			m_turretSprite.rotate(-1.0f);
			break;
		case sf::Keyboard::Space:
			m_fireRequest = true;
			break;
		default:
			break;
		}
	}
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{	
	// Is the circle inside the vision cone
	// Taking the perspective from the vision cone end, looking towards the tank
	// If the circle is left of the left line and right of the right line, it is inside the cone.?
	bool leftOfLeftLine = isLeft(m_visionConeLeftEnd, m_visionConeLeft, m_circleShape.getPosition());
	bool rightOfRightLine = isRight(m_visionConeRightEnd, m_visionConeRight, m_circleShape.getPosition());
	
	if (leftOfLeftLine && rightOfRightLine)
	{
		m_circleShape.setFillColor(sf::Color::Green);
	}
	else
	{
		m_circleShape.setFillColor(sf::Color::Red);
	}


	m_particleSystem.update(dt);
	// If space has been pressed (fire request)
	if (m_fireRequest)
	{
		// Initiate the timer
		m_timer.start();
	
		double rotation = m_turretSprite.getRotation();
		sf::Vector2f turretPos = m_turretSprite.getPosition();
	
		// We can calculate the correct position to draw the projectile
		//  using:
		// x = radius * sin(θ), y = radius * cos(θ)
		// where r = turret radius, θ = turret rotation
		//
		// Note that in SFML the y-axis is inverted so we need
		// to change the above to:
		// x = radius * cos(θ), y = radius * sin(θ)

		m_startPoint = sf::Vector2f(
			turretPos.x + 1 * std::cos(rotation * MathUtility::DEG_TO_RAD),
			turretPos.y + 1 * std::sin(rotation * MathUtility::DEG_TO_RAD)
		);

		m_circleShape.setPosition(m_startPoint);
		
		// Class Exercise: Why does the commented line below not work?
		//m_startPoint = thor::unitVector(m_startPoint);
		m_startPoint = thor::unitVector(m_startPoint - m_turretSprite.getPosition());
		m_fireRequest = false;

		// Generate some particles
		m_particleSystem.generateParticles(m_turretSprite.getPosition().x + m_startPoint.x * TURRET_LENGTH, 
			                               m_turretSprite.getPosition().y + m_startPoint.y * TURRET_LENGTH);

	}
	// If the timer has elapsed, start translating the circle along the 
	//  direction vector.
	if (m_timer.isExpired())
	{
		m_circleShape.move(m_startPoint.x * PROJECTILE_SPEED * (dt / 1000), 
						   m_startPoint.y * PROJECTILE_SPEED * (dt / 1000));
	}
	else if (m_timer.isRunning())
	{
		float timeRemainPerCent = m_timer.getRemainingTime().asMilliseconds() / TIMER_DURATION;
		m_rectShape.setScale(timeRemainPerCent, 1);
		m_rectShape.setTextureRect(
			sf::IntRect(0, 0, m_timingBarTexture.getSize().x * timeRemainPerCent, 
			                  m_timingBarTexture.getSize().y)
		);
	}
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	m_window.draw(m_tankBaseSprite);
	m_window.draw(m_turretSprite);
	m_window.draw(m_circleShape);
	m_window.draw(m_rectShape);
	m_window.draw(m_arrowLeft);
	m_window.draw(m_arrowRight);
	m_particleSystem.render(m_window);
	m_window.display();
}

void Game::initTankSprites()
{
	// Initialise the tank base
	m_tankBaseSprite.setTexture(m_spriteSheetTexture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBaseSprite.setTextureRect(baseRect);
	m_tankBaseSprite.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turretSprite.setTexture(m_spriteSheetTexture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turretSprite.setTextureRect(turretRect);
	m_turretSprite.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);

	// Set initial positions
	m_tankBaseSprite.setPosition(400, 400);
	m_turretSprite.setPosition(400, 400);

}

void Game::setVisionCone(float t_angle)
{
	m_visionConeLeft = m_turretSprite.getPosition();	
	m_visionConeRight = m_turretSprite.getPosition();

	// Setup the arrow visualisation
	m_arrowLeft.setStyle(thor::Arrow::Style::Line);
	m_arrowLeft.setColor(sf::Color::Green);
	m_arrowLeft.setPosition(m_visionConeLeft);
	m_arrowRight.setStyle(thor::Arrow::Style::Line);
	m_arrowRight.setColor(sf::Color::Green);
	m_arrowRight.setPosition(m_visionConeRight);
	      
	// Calculate the vector that points along the left side of the vision cone.
	sf::Vector2f visionConeDirLeft = VISION_CONE_LENGTH * thor::rotatedVector(m_visionConeDir, -t_angle);
	// Calculate the vector that points along the right side of the vision cone.
	sf::Vector2f visionConeDirRight = VISION_CONE_LENGTH * thor::rotatedVector(m_visionConeDir, t_angle);

	// Get the end point of the left vector.
	m_visionConeLeftEnd = sf::Vector2f(m_visionConeLeft.x + visionConeDirLeft.x, m_visionConeLeft.y + visionConeDirLeft.y);
	// Get the end point of the right vector.
	m_visionConeRightEnd = sf::Vector2f(m_visionConeRight.x + visionConeDirRight.x, m_visionConeRight.y + visionConeDirRight.y);

	// Setup the thor arrow to visualise
	m_arrowLeft.setDirection(visionConeDirLeft);
	m_arrowRight.setDirection(visionConeDirRight);

}

bool Game::isRight(sf::Vector2f t_linePoint1, sf::Vector2f t_linePoint2, sf::Vector2f t_point) const
{
	return (
		(t_linePoint2.x - t_linePoint1.x) * (t_point.y - t_linePoint1.y) -
		(t_linePoint2.y - t_linePoint1.y) * (t_point.x - t_linePoint1.x)
		) > 0;
}


bool Game::isLeft(sf::Vector2f t_linePoint1, sf::Vector2f t_linePoint2, sf::Vector2f t_point) const
{
	return (
		(t_linePoint2.x - t_linePoint1.x) * (t_point.y - t_linePoint1.y) -
		(t_linePoint2.y - t_linePoint1.y) * (t_point.x - t_linePoint1.x)
		) < 0;
}