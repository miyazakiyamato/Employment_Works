#include "StageManager.h"
#include "SmallDrone.h"

#include "ChargeGun.h"

void StageManager::Initialize(BulletManager* bulletManager, ParticleSystem* particleSystem) {
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
		if (objectData->typeName == "ARMATURE") {
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
		if (objectData->typeName == "EnemySpawn") {
			std::unique_ptr<BaseEnemy> enemy = std::make_unique<SmallDrone>();
			enemy->SetBulletManager(bulletManager);
			enemy->SetParticleSystem(particleSystem);
			enemy->SetPlayer(player_.get());
			enemy->Initialize();
			enemy->SetPosition(objectData->translation);
			enemies_.push_back(std::move(enemy)); // Use member enemies_
		}
		// レールカメラポイント
		if (objectData->typeName == "ControlPointSpawn") {
			railCameraPoints.push_back(objectData->translation);
		}
	}

	railCamera_->SetControlPoints(railCameraPoints);
	
	//プレイヤーにレールカメラ情報をセット
	player_->SetRailCamera(railCamera_.get());
	player_->SetParent(railCamera_->GetObject3d());
	player_->SetCamera(railCamera_->GetCamera());
}

void StageManager::SetStageCollisions(CollisionManager* collisionManager) {
	//全てのコライダーを衝突マネージャのリストに登録する
	collisionManager->AddCollider(player_.get());
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		collisionManager->AddCollider(enemy.get());
	}
}

void StageManager::Finalize() {
	player_.reset();
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		enemy.reset();
	}
	skydome_.reset();
	ground_.reset();
	railCamera_.reset();
	object3ds_.clear();
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
}
