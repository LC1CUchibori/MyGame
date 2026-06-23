#pragma once
#include "Particle.h"
#include <memory>
#include <vector>
#include <KamataEngine.h>

class ParticlePool
{
public:
    void Initialize(
        KamataEngine::Model* model,
        KamataEngine::Camera* camera,
        uint32_t maxCount);

    void Create(const KamataEngine::Vector3& pos);

    void Update();

    void Draw();

private:
    std::vector<std::unique_ptr<Particle>> particles_;

    KamataEngine::Model* model_ = nullptr;
    KamataEngine::Camera* camera_ = nullptr;
};