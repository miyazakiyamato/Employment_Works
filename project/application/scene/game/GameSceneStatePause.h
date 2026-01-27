#pragma once
#include "BaseSceneState.h"
#include <memory>

class GameScene;

class GameSceneStatePause : public BaseSceneState<GameScene> {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	void Initialize(GameScene* gameScene) override;

	/// <summary>
	/// 初期化（前のステートを引き継ぐ）
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	/// <param name="previousState">中断したステート</param>
	void Initialize(GameScene* gameScene, std::unique_ptr<BaseSceneState<GameScene>> previousState);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	// 中断したステート
	std::unique_ptr<BaseSceneState<GameScene>> previousState_;
};
