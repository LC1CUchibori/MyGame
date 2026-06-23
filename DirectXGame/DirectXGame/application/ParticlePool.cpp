#include "ParticlePool.h"

using namespace KamataEngine;

void ParticlePool::Initialize(Model* model,Camera* camera,uint32_t maxCount)
{
    model_ = model;
    camera_ = camera;

    particles_.reserve(maxCount);

    for (uint32_t i = 0; i < maxCount; i++)
    {
        particles_.push_back(
            std::make_unique<Particle>());
    }
}

void ParticlePool::Create(const Vector3& pos)
{
    for (auto& particle : particles_)
    {
        if (!particle->IsAlive())
        {
            particle->Initialize(
                model_,
                camera_,
                pos);

            return;
        }
    }
}

void ParticlePool::Update()
{
    for (auto& particle : particles_)
    {
        if (particle->IsAlive())
        {
            particle->Update();
        }
    }
}

void ParticlePool::Draw()
{
    for (auto& particle : particles_)
    {
        if (particle->IsAlive())
        {
            particle->Draw();
        }
    }
}