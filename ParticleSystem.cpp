#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	// Load the texture needed for the particle system
	if (!m_particleTexture.loadFromFile("./resources/assets/graphics/SmokeTexture.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}
}

void ParticleSystem::initParticleSystem()
{
	m_particleSystem.setTexture(m_particleTexture);
	// Create 3 particle emitters.
	for (int i = 0; i < 3; i++)
	{
		thor::UniversalEmitter emitter;
		// Configure an emission rate of 30 particles per second.
		emitter.setEmissionRate(30);
		// Set lifetime of individual particles to a value between 200 and 400 milliseconds.
		emitter.setParticleLifetime(thor::Distributions::uniform(sf::milliseconds(200), sf::milliseconds(400)));
		m_emitters.push_back(emitter);
	}
}

////////////////////////////////////////////////////////////
void ParticleSystem::generateParticles(int t_x, int t_y)
{
	// Emit particles in given circle at x,y position with radius of 10.
	m_emitters.at(m_emitterIndex).setParticlePosition(thor::Distributions::circle(sf::Vector2f(t_x, t_y), 10));
	// Add an emitter...this emitter will be removed after 400 milliseconds.
	m_particleSystem.addEmitter(thor::refEmitter(m_emitters.at(m_emitterIndex)), sf::milliseconds(400));
	m_emitterIndex = (m_emitterIndex + 1) % 3;
}

void ParticleSystem::update(double dt)
{
	// Update particle system (needs delta time between frames)
	m_particleSystem.update(sf::Time(sf::milliseconds(dt)));
}

void ParticleSystem::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_particleSystem);
}
