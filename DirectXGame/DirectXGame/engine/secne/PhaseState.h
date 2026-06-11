#pragma once

class GameScene;

class PhaseState {
public:
	virtual ~PhaseState() = default;

	virtual void Enter(GameScene* scene) = 0;
	virtual void Update(GameScene* scene) = 0;
};