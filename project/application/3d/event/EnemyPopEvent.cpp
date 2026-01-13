#include "EnemyPopEvent.h"
#include "SmallDrone.h"
#include "StageManager.h"

void EnemyPopEvent::Initialize() {
	BaseEventObject::Initialize();
	Collider::SetRadius(10.0f);
}

void EnemyPopEvent::OnCollision(Collider* other) {
	// プレイヤーと衝突したら
	if (other->GetTypeID() == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		if (stageManager_) {
			// 登録されたデータに基づいてエネミーを生成
			for (const auto& data : enemySpawnDatas_) {
				std::unique_ptr<BaseEnemy> enemy = std::make_unique<SmallDrone>();

				Vector3 spawnPosition = object3d_->GetTranslate() + data.translation;
				
				stageManager_->AddEnemy(std::move(enemy), spawnPosition);
			}
			
			isFinished_ = true;
		}
	}
}
