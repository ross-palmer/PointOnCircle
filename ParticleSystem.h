#pragma once

#include <Thor/Particles.hpp>
#include <Thor/Math/Distributions.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class ParticleSystem
{
public:
	ParticleSystem();

	void initParticleSystem();

	void generateParticles(int t_x, int t_y);

	void update(double dt);

	void render(sf::RenderWindow& t_window);

private:	
	// Thor's particle system instance.
	thor::ParticleSystem m_particleSystem;
	// The texture for this particle system.
	sf::Texture m_particleTexture;
	// A collection of particle emitters.
	std::vector<thor::UniversalEmitter> m_emitters;
	// The index of the next available emitter.
	int m_emitterIndex{ 0 };
};