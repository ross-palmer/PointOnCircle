#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Shapes.hpp>

#include "ScreenSize.h"
#include "MathUtility.h"
#include "Thor/Time.hpp"
#include "ParticleSystem.h"

/// <summary>
/// @author RP
/// @date March 2021
/// @version 1.0
/// 
/// </summary>
/// <summary>
/// @brief Main class for the SFML Playground project.
/// 
/// 
/// This will be a single class framework for learning about SFML. 
/// Example usage:
///		Game game;
///		game.run();
/// </summary>

class Game
{
public:
	/// <summary>
	/// @brief Default constructor that initialises the SFML window, 
	///   and sets vertical sync enabled. 
	/// </summary>
	Game();

	/// <summary>
	/// @brief the main game loop.
	/// 
	/// A complete loop involves processing SFML events, updating and drawing all game objects.
	/// The actual elapsed time for a single game loop results (lag) is stored. If this value is 
	///  greater than the notional time for one loop (MS_PER_UPDATE), then additional updates will be 
	///  performed until the lag is less than the notional time for one loop.
	/// The target is one update and one render cycle per game loop, but slower PCs may 
	///  perform more update than render operations in one loop.
	/// </summary>
	void run();

	bool isLeft(sf::Vector2f t_linePoint1, sf::Vector2f t_linePoint2, sf::Vector2f t_point) const;
	bool isRight(sf::Vector2f t_linePoint1, sf::Vector2f t_linePoint2, sf::Vector2f t_point) const;

protected:
	/// <summary>
	/// @brief Placeholder to perform updates to all game objects.
	/// </summary>
	/// <param name="time">update delta time</param>
	void update(double dt);

	/// <summary>
	/// @brief Draws the background and foreground game objects in the SFML window.
	/// The render window is always cleared to black before anything is drawn.
	/// </summary>
	void render();

	/// <summary>
	/// @brief Checks for events.
	/// Allows window to function and exit. 
	/// Events are passed on to the Game::processGameEvents() method.
	/// </summary>	
	void processEvents();

	/// <summary>
	/// @brief Handles all user input.
	/// </summary>
	/// <param name="event">system event</param>
	void processGameEvents(sf::Event&);

	void initTankSprites();

	/// <summary>
	/// @brief Helper function to configure vision cone 
	/// </summary>
	/// <param name="t_angle">The vision cone width in degrees</param>
	void setVisionCone(float t_angle);

	// main window
	sf::RenderWindow m_window;

	// Custom particleSystem 
	ParticleSystem m_particleSystem;

	sf::Texture m_spriteSheetTexture;
	sf::Texture m_tankBaseTexture;
	sf::Texture m_turretTexture;
	sf::Texture m_timingBarTexture;
	sf::Sprite m_tankBaseSprite;
	sf::Sprite m_turretSprite;

	// To represent the projectile.
	sf::CircleShape m_circleShape;
	// Constant for projectile speed.
	static constexpr float PROJECTILE_SPEED{ 200.0f };

	// Where to draw the projectile.
	sf::Vector2f m_startPoint;

	bool m_fireRequest{ false };

	thor::Timer m_timer;
	static constexpr float TIMER_DURATION = 500.0f;

	// Approx. length of turret
	static const int TURRET_LENGTH = 50;
	
	// A rectangle for the timing bar
	sf::RectangleShape m_rectShape;
	static const int TIMING_BAR_WIDTH{ 100 };

	// Vision cone vector...initially points along the x axis.
	sf::Vector2f m_visionConeDir{ 1, 0 };

	// Vision cone visualisation...vision cone origin
	sf::Vector2f m_visionConeLeft;
	// End point of left line
	sf::Vector2f m_visionConeLeftEnd;

	sf::Vector2f m_visionConeRight;
	// End point of right line
	sf::Vector2f m_visionConeRightEnd;

	thor::Arrow m_arrowLeft;
	thor::Arrow m_arrowRight;

	// Vision Cone max length.
	static constexpr float VISION_CONE_LENGTH{ 200.0f };

};
