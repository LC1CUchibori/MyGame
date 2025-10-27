#pragma once
#include <KamataEngine.h>

class SharkTop {
public:
	void Initialize(KamataEngine::Model* model);
	void Update();
	void Draw(KamataEngine::Camera* camera);

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* sharkTopModel_ = nullptr;

	float rotationTimer_ = 0.0f;
	bool isRotationStopped_ = false;
	bool isReturning_ = false;
	bool hasReturned_ = false;

	KamataEngine::Vector3 initialPosition_;
};
