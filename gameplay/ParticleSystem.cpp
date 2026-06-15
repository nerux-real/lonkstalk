#include "ParticleSystem.h"

void ParticleSystem::spawnParticles(int x, int y, Uint8 r, Uint8 g, Uint8 b, int count){
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> speedDist(100.0f, 400.0f);

    for(int i=0; i<count; i++){
        Particle p;
        p.x = (float)x;
        p.y = (float)y;
        p.r = r; p.g = g; p.b = b; p.a = 255;
        p.maxLifetime = 500.0f;
        p.lifetime = p.maxLifetime;

        float angle = angleDist(m_gen);
        float speed = speedDist(m_gen);
        p.vx = cos(angle) * speed;
        p.vy = sin(angle) * speed;

        m_particles.push_back(p);
    }
}

void ParticleSystem::updateParticles(float deltaMs){
    for(auto &p : m_particles){
        p.x += p.vx * (deltaMs / 1000.0f);
        p.y += p.vy * (deltaMs / 1000.0f);
        p.lifetime -= deltaMs;
        if(p.lifetime < 0) p.lifetime = 0;
        p.a = (Uint8)((p.lifetime / p.maxLifetime) * 255);
    }
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
        [](const Particle &p){ return p.lifetime <= 0; }),
        m_particles.end()
    );
}

void ParticleSystem::renderParticles(SDL_Renderer *renderer){
    for(const auto &p : m_particles){
        SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, p.a);
        SDL_Rect rect = {(int)p.x, (int)p.y, 4, 4};
        SDL_RenderFillRect(renderer, &rect);
    }
}