#include "StageManager.h"
#include "SmallDrone.h"

#include "ChargeGun.h"
#include "EnemyPopEvent.h"

void StageManager::Initialize(BulletManager* bulletManager, ParticleSystem* particleSystem) {
	bulletManager_ = bulletManager;
	particleSystem_ = particleSystem;

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// 地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	// レールカメラ
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({ 0.0f, 5.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
	std::vector<Vector3> railCameraPoints = {};

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetParticleSystem(particleSystem);
	auto weapon = std::make_unique<ChargeGun>();
	weapon->Initialize();
	weapon->SetBulletManager(bulletManager);
	player_->SetWeapon(std::move(weapon));

	// レベルデータマネージャの生成
	levelDataManager_ = std::make_unique<LevelDataManager>();
	// レベルデータの読み込み取得
	levelDataManager_->LoadJsonFile("level1");
	LevelDataManager::LevelData* levelData = levelDataManager_->GetObjectData("level1");

	for (const std::unique_ptr<ObjectData>& objectData : *levelData) {
		if (objectData->typeName == "MESH") {
			std::unique_ptr<Object3d> object3d(new Object3d);
			object3d->Initialize();
			object3d->SetScale(objectData->scaling);
			object3d->SetRotate(objectData->rotation);
			object3d->SetTranslate(objectData->translation);
			if (!objectData->fileName.empty()) {
				object3d->SetModel(objectData->fileName);
			}
			object3ds_.push_back(std::move(object3d));
		}
		else if (objectData->typeName == "ARMATURE") {
			for (const std::unique_ptr<ObjectData>& childData : objectData->children) {
				if (childData->typeName == "MESH") {
					std::unique_ptr<Object3d> object3d(new Object3d);
					object3d->Initialize();
					object3d->SetTranslate(objectData->translation);
					if (!childData->fileName.empty()) {
						object3d->SetModel(childData->fileName);
						object3d->SetAnimation(childData->fileName, true);
					}
					object3ds_.push_back(std::move(object3d));
				}
			}
		}
		// エネミー
		else if (objectData->typeName == "EnemySpawn") {
			std::unique_ptr<BaseEnemy> enemy = std::make_unique<SmallDrone>();
			enemy->SetBulletManager(bulletManager);
			enemy->SetParticleSystem(particleSystem);
			enemy->SetPlayer(player_.get());
			enemy->Initialize();
			enemy->SetPosition(objectData->translation);
			enemies_.push_back(std::move(enemy));
		}
		// 敵出現イベント
		else if (objectData->typeName == "EnemyPopEvent") {
			std::unique_ptr<EnemyPopEvent> event = std::make_unique<EnemyPopEvent>();
			event->Initialize();
			event->SetPosition(objectData->translation);
			event->GetObject3d()->SetRotate(objectData->rotation);
			event->GetObject3d()->SetScale(objectData->scaling);
			event->SetStageManager(this);

			// 子供のデータを解析してスポーンデータを追加
			for (const std::unique_ptr<ObjectData>& childData : objectData->children) {
				if (childData->typeName == "EnemySpawn") {
					EnemySpawnData data;
					// 親からの相対位置
					// Blenderの構造上、子供のtranslationは親からの相対位置になっているはず
					data.translation = childData->translation;
					data.rotation = childData->rotation.ToQuaternion();
					event->AddEnemySpawnData(data);
				}
			}

			AddEventObject(std::move(event));
		}

		// カメラ
		else if (objectData->typeName == "CAMERA") {
			for (const std::unique_ptr<ObjectData>& child : objectData->children) {
				if (child->typeName == "Rail") {
					for (const std::unique_ptr<ObjectData>& grandChild : child->children) {
						if (grandChild->typeName == "ControlPointSpawn") {
							// 階層構造の座標を合算してワールド座標を計算 (簡易実装: 回転・スケール無視で座標加算)
							Vector3 worldPos = objectData->translation + child->translation + grandChild->translation;
							railCameraPoints.push_back(worldPos);
						}
					}
				}
			}
		}
	}

	railCamera_->SetControlPoints(railCameraPoints);
	
	//プレイヤーにレールカメラ情報をセット
	player_->SetRailCamera(railCamera_.get());
	player_->SetParent(railCamera_->GetObject3d());
	player_->SetCamera(railCamera_->GetCamera());

}

void StageManager::AddEnemy(std::unique_ptr<BaseEnemy> enemy, const Vector3& position) {
	enemy->SetBulletManager(bulletManager_);
	enemy->SetParticleSystem(particleSystem_);
	enemy->SetPlayer(player_.get());
	enemy->Initialize();
	enemy->SetPosition(position);
	enemies_.push_back(std::move(enemy));
}

void StageManager::AddEventObject(std::unique_ptr<BaseEventObject> eventObject) {
	eventObjects_.push_back(std::move(eventObject));
}

void StageManager::SetStageCollisions(CollisionManager* collisionManager) {
	//全てのコライダーを衝突マネージャのリストに登録する
	collisionManager->AddCollider(player_.get());
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		collisionManager->AddCollider(enemy.get());
	}
	for (std::unique_ptr<BaseEventObject>& eventObject : eventObjects_) {
		collisionManager->AddCollider(eventObject.get());
	}
}



void StageManager::Update() {
	// 天球
	skydome_->Update();
	// 地面
	ground_->Update();
	
	// レールカメラ
	railCamera_->Update();

	// プレイヤー
	if (player_) {
		player_->Update();
	}

	// エネミー
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		enemy->Update();
	}
	
	// 死んだエネミーを削除
	enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](std::unique_ptr<BaseEnemy>& enemy) {
		if (!enemy->GetIsAlive()) {
			enemy.reset();
			return true;
		}
		return false;
		}), enemies_.end());

	// イベントオブジェクト
	for (std::unique_ptr<BaseEventObject>& eventObject : eventObjects_) {
		eventObject->Update();
	}
	// 終了したイベントを削除
	eventObjects_.erase(std::remove_if(eventObjects_.begin(), eventObjects_.end(), [](std::unique_ptr<BaseEventObject>& event) {
		if (event->GetIsFinished()) {
			event.reset();
			return true;
		}
		return false;
		}), eventObjects_.end());

	// Object3d
	for (std::unique_ptr<Object3d>& object3d : object3ds_) {
		object3d->Update();
	}
}

void StageManager::Draw() {
	// 天球
	skydome_->Draw();
	// 地面
	ground_->Draw();
	
	// Object3d
	for (std::unique_ptr<Object3d>& object3d : object3ds_) {
		object3d->Draw();
	}

	// レールカメラ
	railCamera_->Draw();

	// プレイヤー
	if (player_) {
		player_->Draw();
	}

	// エネミー
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		enemy->Draw();
	}

	// イベントオブジェクト
	for (std::unique_ptr<BaseEventObject>& eventObject : eventObjects_) {
		eventObject->Draw();
	}
}
