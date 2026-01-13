#pragma once
#include "BaseEventObject.h"

class StageManager;
/// <summary>
/// 敵出現イベント
/// </summary>
class EnemyPopEvent : public BaseEventObject {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(Collider* other) override;

	/// <summary>
	/// ステージマネージャーのセット
	/// </summary>
	/// <param name="stageManager"></param>
	void SetStageManager(StageManager* stageManager) { stageManager_ = stageManager; }

private:
	StageManager* stageManager_ = nullptr;
};
