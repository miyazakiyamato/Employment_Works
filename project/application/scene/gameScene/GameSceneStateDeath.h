#pragma once
#include "BaseSceneState.h"
#include "PlayerDeathUI.h"
#include <memory>

class GameSceneStateDeath : public BaseSceneState {
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

private:
	std::unique_ptr<PlayerDeathUI> playerDeathUI_;

	//フェードの持続時間
	float duration_ = 1.0f;
	//経過時間カウンター
	float counter_ = 0.0f;
};
