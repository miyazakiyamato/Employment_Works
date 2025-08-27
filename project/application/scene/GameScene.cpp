#include "GameScene.h"
#include <imgui.h>
#include "SceneManager.h"
#include "Input.h"
#include "CameraManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "ParticleManager.h"
#include "GlobalVariables.h"
#include "TimeManager.h"
#include "Line3D.h"
#include "HitEffect.h"
#include <numbers>

void GameScene::Initialize(){
	BaseScene::Initialize();

	//開発用のUIの処理。
	// ウインドウのサイズを固定する
	ImGui::SetNextWindowSize(ImVec2(1280, 40));
	// ウインドウの位置を設定する
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	CameraManager::GetInstance()->SetCamera("Camera2");
	CameraManager::GetInstance()->FindCamera("Camera2");
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.3f,-0.8f,0.0f });
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 8.0f,4.0f,-8.0f });

	CameraManager::GetInstance()->FindCamera("default");

	AudioManager::GetInstance()->LoadWave("maou_se_system48.wav");
	//AudioManager::GetInstance()->LoadMP3("audiostock_1420737.mp3");

	//衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	ModelManager::GetInstance()->LoadModel("plane/plane.obj");
	ModelManager::GetInstance()->LoadModel("fence/fence.obj");
	ModelManager::GetInstance()->LoadModel("axis/axis.obj");
	ModelManager::GetInstance()->LoadModel("sphere/sphere.obj");
	ModelManager::GetInstance()->LoadModel("terrain/terrain.obj");
	ModelManager::GetInstance()->LoadModel("skybox");
	ModelManager::GetInstance()->LoadModel("sword/sword.obj");
	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("ground/ground.obj");
	ModelManager::GetInstance()->LoadModel("airship/airship.obj");
	ModelManager::GetInstance()->LoadModel("cube/cube.obj");

	/*ModelManager::GetInstance()->LoadModel("AnimatedCube/AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadAnimation("AnimatedCube/AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadModel("simpleSkin/simpleSkin.gltf");
	ModelManager::GetInstance()->LoadAnimation("simpleSkin/simpleSkin.gltf");
	ModelManager::GetInstance()->LoadModel("human/sneakWalk.gltf");
	ModelManager::GetInstance()->LoadAnimation("human/sneakWalk.gltf");
	ModelManager::GetInstance()->LoadModel("human/walk.gltf");
	ModelManager::GetInstance()->LoadAnimation("human/walk.gltf");*/
	ModelManager::GetInstance()->LoadModel("BrainStem/BrainStem.gltf");
	ModelManager::GetInstance()->LoadAnimation("BrainStem/BrainStem.gltf");

	TextureManager::GetInstance()->LoadTexture("circle2.png");
	TextureManager::GetInstance()->LoadTexture("gradationLine.png");
	TextureManager::GetInstance()->LoadTexture("reticle.png");
	TextureManager::GetInstance()->LoadTexture("rostock_laage_airport_4k.dds");

	////skybox
	//std::unique_ptr<Object3d> object3d(new Object3d);
	//object3d->Initialize();
	//object3d->SetScale({ 1000.0f,1000.0f,1000.0f });
	//object3d->SetModel("skybox");
	//object3d->SetTexture("rostock_laage_airport_4k.dds");
	//object3ds_.push_back(std::move(object3d));
	//std::unique_ptr<Object3d> object3d2(new Object3d);
	//object3d2->Initialize();
	//object3d2->SetTranslate({-1.0f,0.0f,0.0f});
	//object3d2->SetModel("BrainStem/BrainStem.gltf");
	//object3d2->SetAnimation("BrainStem/BrainStem.gltf", true);
	////object3d2->SetEnvironmentTexture("rostock_laage_airport_4k.dds");
	//object3ds_.push_back(std::move(object3d2));
	////レベルデータマネージャの生成
	//levelDataManager_ = std::make_unique<LevelDataManager>();
	////レベルデータの読み込み取得
	//levelDataManager_->LoadJsonFile("level1"); 
	//LevelDataManager::LevelData* levelData = levelDataManager_->GetObjectData("level1");
	//for (const std::unique_ptr<ObjectData>& objectData : *levelData) {
	//	if (objectData->typeName == "MESH") {
	//		std::unique_ptr<Object3d> object3d(new Object3d);
	//		object3d->Initialize();
	//		object3d->SetScale(objectData->scaling);
	//		object3d->SetRotate(objectData->rotation);
	//		object3d->SetTranslate(objectData->translation);
	//		if (!objectData->fileName.empty()) {
	//			object3d->SetModel(objectData->fileName);
	//			//object3d->SetEnvironmentTexture("rostock_laage_airport_4k.dds");
	//		}
	//		object3ds_.push_back(std::move(object3d));
	//	}
	//	if (objectData->typeName == "ARMATURE") {
	//		for (const std::unique_ptr<ObjectData>&childData : objectData->children) {
	//			if (childData->typeName == "MESH") {
	//				std::unique_ptr<Object3d> object3d(new Object3d);
	//				object3d->Initialize();
	//				object3d->SetTranslate(objectData->translation);
	//				if (!childData->fileName.empty()) {
	//					object3d->SetModel(childData->fileName);
	//					object3d->SetAnimation(childData->fileName, true);
	//					//object3d->SetEnvironmentTexture("rostock_laage_airport_4k.dds");
	//				}
	//				object3ds_.push_back(std::move(object3d));
	//			}
	//		}
	//	}
	//}
	//std::unique_ptr<Object3d> object3d3(new Object3d);
	//object3d3->Initialize();
	//object3d3->SetParent(object3ds_[2].get());
	//std::unique_ptr<Object3d> object3d4(new Object3d);
	//object3d4->Initialize();
	//object3d4->SetTranslate({ 0.0f,7.0f,2.0f });
	//object3d4->SetRotate({ 0.0f,0.0f,1.57f });
	//object3d4->SetScale({ 10.0f,10.0f,10.0f });
	//object3d4->SetModel("sword/sword.obj");
	//object3d4->SetParent(object3d3.get());
	//object3ds_.push_back(std::move(object3d3));
	//object3ds_.push_back(std::move(object3d4));
	/*object3ds_[0]->SetModel("AnimatedCube/AnimatedCube.gltf");
	object3ds_[0]->SetAnimation("AnimatedCube/AnimatedCube.gltf",true);*/
	/*object3ds_[0]->SetModel("simpleSkin/simpleSkin.gltf");
	object3ds_[0]->SetAnimation("simpleSkin/simpleSkin.gltf",true);*/
	/*object3ds_[1]->SetTranslate({ -1,0,0 });
	object3ds_[1]->SetRotate({ 0,3.14f,0 });
	object3ds_[1]->SetModel("terrain/terrain.obj");*/
	//object3ds_[1]->SetModel("plane/plane.gltf");
	//object3ds_[1]->SetModel("axis/axis.obj");
	/*object3ds_[1]->SetModel("human/sneakWalk.gltf");
	object3ds_[1]->SetAnimation("human/sneakWalk.gltf", true);
	object3ds_[1]->SetTranslate({ 1,0,0 });
	object3ds_[1]->SetRotate({ 0,3.14f,0 });
	object3ds_[2]->SetModel("human/walk.gltf");
	object3ds_[2]->SetAnimation("human/walk.gltf", true);
	object3ds_[2]->SetTranslate({ 0,0,0 });
	object3ds_[2]->SetRotate({ 0,3.14f,0 });*/
	//
	isAccelerationField = false;
	accelerationField_.reset(new AccelerationField);

	particleSystem_.reset(new ParticleSystem);
	std::unique_ptr<BaseParticleEmitter> hitEffect = std::make_unique<HitEffect>();
	particleSystem_->CreateParticleEmitter("hitEffect", std::move(hitEffect));

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
	//地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	//バレットマネージャーの生成
	bulletManager_ = std::make_unique<BulletManager>();
	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetBulletManager(bulletManager_.get());
	
	//エネミー
	for (uint32_t i = 0; i < 5; ++i) {
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->SetBulletManager(bulletManager_.get());
		enemy->SetParticleSystem(particleSystem_.get());
		enemy->SetPlayer(player_.get());
		enemy->Initialize();
		if (i % 2 == 0) {
			enemy->SetPosition({ 5.0f + float(i * 10),1.0f + float(i * 2), 90.0f + float(i * 30) });
		} else {
			enemy->SetPosition({ 5.0f + -float(i * 10),-1.0f + float(i * 2), 90.0f + float(i * 30) });
		}
		enemies_.push_back(std::move(enemy));
	}
	//
	isAccelerationField = false;
	accelerationField_.reset(new AccelerationField);

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({ 0.0f, 5.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
	player_->SetParent(railCamera_->GetObject3d());
	player_->SetCamera(railCamera_->GetCamera());


	//スプライトの初期化
	for (uint32_t i = 0; i < 0; ++i) {
		std::unique_ptr<Sprite> sprite(new Sprite);
		sprite->Initialize("uvChecker.png");
		sprite->SetPosition({ 100 + 200.0f * float(i), 100 });
		sprite->SetSize({ 100.0f,100.0f });
		sprites_.push_back(std::move(sprite));
	}
	/*sprites_[0]->SetTextureSize({ 64.0f,64.0f });
	sprites_[1]->SetTexture("monsterBall.png");
	sprites_[1]->SetIsFlipX(true);
	sprites_[2]->SetIsFlipY(true);
	sprites_[3]->SetIsFlipX(true);
	sprites_[3]->SetIsFlipY(true);*/
}

void GameScene::Finalize(){
	//解放
	railCamera_.reset();

	bulletManager_->Finalize();
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy.reset();
	}
	player_.reset();
	ground_.reset();
	skydome_.reset();

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite.reset();  // メモリを解放する
	}
	BaseScene::Finalize();
}

void GameScene::Update(){
	BaseScene::Update();

#ifdef _DEBUG
	//// ウインドウフラグに NoResize を指定
	//ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoResize);
	//ImGui::ShowDemoWindow();
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->Update();
	std::string groupName = "";
	if (ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			input_->ImGuiUpdate();
			CameraManager::GetInstance()->ImGuiUpdate();

			LightManager::GetInstance()->ImGuiUpdate();
			
			/*size_t object3dCount = 0;
			for (std::unique_ptr<Object3d>& object3d : object3ds_) {
				std::string objectName = ("Object3d" + std::to_string(object3dCount)).c_str();
				object3d->ImGuiUpdate(objectName);

				object3dCount++;
			}*/
			ParticleManager::GetInstance()->UpdateGlobalVariables();
			//groupName = "Particle";
			
			groupName = "Sprite";
			uint32_t spriteIDIndex = 0;
			for (std::unique_ptr<Sprite>& sprite : sprites_) {
				std::string spriteName = ("Sprite" + std::to_string(spriteIDIndex)).c_str();
				sprite->ImGuiUpdate(spriteName);
				++spriteIDIndex;
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
#endif //_DEBUG
	railCamera_->Update();
	CameraManager::GetInstance()->GetCamera()->Update();

#ifdef _DEBUG
	// デバッグ用にワールドトランスフォームの更新
	collisionManager_->UpdateWorldTransform();
#endif //_DEBUG

	//天球
	skydome_->Update();
	//地面
	ground_->Update();
	//プレイヤー
	player_->Update();

	//エネミー
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update();
	}
	bulletManager_->Update();

	//当たり判定
	CheckAllCollisions();
	enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](std::unique_ptr<Enemy>& enemy) {
		if (!enemy->GetIsAlive()) {
			enemy.reset();
			return true;
		}
		return false;
		}), enemies_.end());
	if (isAccelerationField) {
		for (std::pair<const std::string, std::unique_ptr<ParticleManager::ParticleGroup>>& pair : ParticleManager::GetInstance()->GetParticleGroups()) {
			ParticleManager::ParticleGroup& group = *pair.second;
			int index = 0;
			for (std::list<ParticleManager::Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
				ParticleManager::Particle& particle = *it;

				if (Collision::IsCollision(accelerationField_->GetAABB(), particle.transform.translate)) {
					particle.velocity += accelerationField_->GetAcceleration() * TimeManager::GetInstance()->deltaTime_;

				}

				++it;
				++index;
			}
		}
	}


	particleSystem_->Update();

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}

	ClearCheck();
}

void GameScene::Draw(){
	//Object3dの描画
	//天球
	skydome_->Draw();
	//地面
	ground_->Draw();
	//プレイヤー
	player_->Draw();

	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw();
	}
	bulletManager_->Draw();
	/*for (std::unique_ptr<Object3d>& object3d : object3ds_) {
		object3d->Draw();
		}*/
	//当たり判定の表示
	collisionManager_->Draw();
	//レールカメラの描画
	railCamera_->Draw();
	//ラインの描画
	//Line3dManager::GetInstance()->DrawLine(object3ds_[0]->GetCenterPosition(), object3ds_[1]->GetCenterPosition(),{1.0f,0.0f,0.0f,1.0f});
	//Line3dManager::GetInstance()->DrawLine(object3ds_[1]->GetCenterPosition(), object3ds_[2]->GetCenterPosition(),{1.0f,0.0f,0.0f,1.0f});
	//Line3dManager::GetInstance()->DrawSphere({ object3ds_[0]->GetCenterPosition(),1.0f }, { 1.0f,0.0f,0.0f,1.0f });
	/*Line3dManager::GetInstance()->DrawSphere({ {},1.0f}, {1.0f,0.0f,0.0f,1.0f},10);
	Line3dManager::GetInstance()->DrawGrid({50.0f,3.0f});*/
	Line3dManager::GetInstance()->Draw();

	//Particleの描画
	particleSystem_->Draw();

	//Spriteの描画
	player_->DrawUi();
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		//sprite->Draw();
	}
}

void GameScene::CheckAllCollisions(){
	//衝突マネージャのリストクリアする
	collisionManager_->Reset();
	//全てのコライダーを衝突マネージャのリストに登録する
	collisionManager_->AddCollider(player_.get());
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
	}
	bulletManager_->AddCollider(collisionManager_.get());
	//リスト内の総当たり判定
	collisionManager_->CheckAllCollisions();
}

void GameScene::ClearCheck(){
	//クリア判定
	/*if (enemies_.empty()) {
		敵がいなくなったらクリア
		
	}*/
	if (!player_->GetIsAlive()) {
		//プレイヤーのHPが0になったらゲームオーバー
		sceneManager_->ChangeScene("GAMEOVER");
	}
	if (railCamera_->GetIsFinished()) {
		//レールカメラの移動が終わったら
		sceneManager_->ChangeScene("CLEAR");
	}
}
