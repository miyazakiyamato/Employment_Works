#include "EnemyPopEvent.h"
#include "SmallDrone.h"
#include "StageManager.h"

void EnemyPopEvent::Initialize() {
	BaseEventObject::Initialize();
}

void EnemyPopEvent::OnCollision(Collider* other) {
	// プレイヤーと衝突したら
	if (other->GetTypeID() == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		if (stageManager_) {
			// SmallDroneを生成
			std::unique_ptr<BaseEnemy> enemy = std::make_unique<SmallDrone>();
			stageManager_->AddEnemy(std::move(enemy), object3d_->GetTranslate() + Vector3(0, 5, 0));
			
			isFinished_ = true;
		}
	}
}
