#pragma once
#include "BaseSceneState.h"

class PauseScene;

class PauseStateSelect : public BaseSceneState<PauseScene> {
public:
	void Initialize(PauseScene* scene) override;
	void Update() override;
	void Draw() override {}
private:
	float inputCooldown_ = 0.0f;
};
