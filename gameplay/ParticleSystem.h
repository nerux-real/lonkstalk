#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include <algorithm>

struct Particle {
    float x,y;
    float vx,vy;
    float lifetime;
    float maxLifetime;
    Uint8 r,g,b,a;
};

class ParticleSystem {
public:
    void spawnParticles(int x, int y, Uint8 r, Uint8 g, Uint8 b, int count);
    void updateParticles(float deltaMs);
    void renderParticles(SDL_Renderer *renderer);
private:
    std::vector<Particle> m_particles;
    std::mt19937 m_gen{std::random_device{}()};
};