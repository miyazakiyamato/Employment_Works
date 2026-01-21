#pragma once
#include "BaseSceneState.h"

class GameSceneStateBattle : public BaseSceneState {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameScene* gameScene) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
};
