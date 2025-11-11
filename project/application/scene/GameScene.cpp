#include "GameScene.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

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
#include "EmitterSphere.h"
#include "PostEffectManager.h"
#include <numbers>
#include "PlayerDeathScene.h"
#include <Easing.h>

void GameScene::Initialize(){
	BaseScene::Initialize();
#ifdef USE_IMGUI
	//開発用のUIの処理。
	// ウインドウのサイズを固定する
	ImGui::SetNextWindowSize(ImVec2(1280, 40));
	// ウインドウの位置を設定する
	ImGui::SetNextWindowPos(ImVec2(0, 0));
#endif // USE_IMGUI

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

	//skybox
	/*std::unique_ptr<Object3d> object3d(new Object3d);
	object3d->Initialize();
	object3d->SetScale({ 1000.0f,1000.0f,1000.0f });
	object3d->SetModel("skybox");
	object3d->SetTexture("rostock_laage_airport_4k.dds");
	object3ds_.push_back(std::move(object3d));
	std::unique_ptr<Object3d> object3d2(new Object3d);
	object3d2->Initialize();
	object3d2->SetTranslate({-1.0f,0.0f,0.0f});
	object3d2->SetModel("BrainStem/BrainStem.gltf");
	object3d2->SetAnimation("BrainStem/BrainStem.gltf", true);*/
	//object3d2->SetEnvironmentTexture("rostock_laage_airport_4k.dds");
	//object3ds_.push_back(std::move(object3d2));

	particleSystem_.reset(new ParticleSystem);
	std::unique_ptr<BaseParticleEmitter> hitEffect = std::make_unique<EmitterSphere>();
	hitEffect->Initialize("hitEffect", 100);
	hitEffect->SetPosition({ 1.0f,1.0f,0.0f });
	hitEffect->SetTexture("circle2.png");
	particleSystem_->SetParticleEmitter(std::move(hitEffect));
	//レールカメラ
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({ 0.0f, 5.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
	//railCamera_->SetSegmentTime(30.0f);
	std::vector<Vector3> railCameraPoints = {};

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


	//レベルデータマネージャの生成
	levelDataManager_ = std::make_unique<LevelDataManager>();
	//レベルデータの読み込み取得
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
				//object3d->SetEnvironmentTexture("rostock_laage_airport_4k.dds");
			}
			object3ds_.push_back(std::move(object3d));
		}
		if (objectData->typeName == "ARMATURE") {
			for (const std::unique_ptr<ObjectData>&childData : objectData->children) {
				if (childData->typeName == "MESH") {
					std::unique_ptr<Object3d> object3d(new Object3d);
					object3d->Initialize();
					object3d->SetTranslate(objectData->translation);
					if (!childData->fileName.empty()) {
						object3d->SetModel(childData->fileName);
						object3d->SetAnimation(childData->fileName, true);
						//object3d->SetEnvironmentTexture("rostock_laage_airport_4k.dds");
					}
					object3ds_.push_back(std::move(object3d));
				}
			}
		}
		//エネミー
		if (objectData->typeName == "EnemySpawn") {
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
			enemy->SetBulletManager(bulletManager_.get());
			enemy->SetParticleSystem(particleSystem_.get());
			enemy->SetPlayer(player_.get());
			enemy->Initialize();
			enemy->SetPosition(objectData->translation);
			enemies_.push_back(std::move(enemy));
		}
		//レールカメラポイント
		if (objectData->typeName == "ControlPointSpawn") {
			railCameraPoints.push_back(objectData->translation);
		}
	}

	railCamera_->SetControlPoints(railCameraPoints);

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
	particleSystem_->Finalize();
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

void GameScene::Update() {
	BaseScene::Update();
	if (!sceneManager_->IsSceneFinished("GAME_START") && sceneManager_->IsSceneAlive("GAME_START")) {
		TimeManager::GetInstance()->deltaTime_ = 0.001f;
	} else {
		TimeManager::GetInstance()->deltaTime_ = TimeManager::GetInstance()->kFlamTime_;
	}
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
			particleSystem_->ImGuiUpdate();
			
			groupName = "Sprite";
			uint32_t spriteIDIndex = 0;
			for (std::unique_ptr<Sprite>& sprite : sprites_) {
				std::string spriteName = ("Sprite" + std::to_string(spriteIDIndex)).c_str();
				sprite->ImGuiUpdate(spriteName);
				++spriteIDIndex;
			}
			PostEffectManager::GetInstance()->ImGuiUpdate();
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
#endif //_DEBUG
	railCamera_->Update();
	CameraManager::GetInstance()->GetCamera()->Update();
	PostEffectManager::GetInstance()->Update();
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

	if (sceneManager_->IsSceneAlive("PLAYER_DEATH")) {
		PlayerDeathScene* playerDeathScene = static_cast<PlayerDeathScene*>(sceneManager_->GetScene("PLAYER_DEATH"));
		float scale = Easing::EaseOutBounce(playerDeathScene->GetCounter() / playerDeathScene->GetDuration(), 1.0f, 0.0f);
		player_->GetObject3d()->SetScale({ scale,scale ,scale });
		static_cast<EmitterSphere*>(particleSystem_->FindEmitter("hitEffect"))->SetTranslate(player_->GetObject3d()->GetCenterPosition());
		if (int(scale * 100.0f) % 5 < 1 && scale > 0.1f) {
			particleSystem_->Emit("hitEffect");
		}/*if (scale == 1) {
			particleSystem_->Emit("hitEffect");
		}*/
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

void GameScene::ClearCheck() {
	//クリア判定
	/*if (enemies_.empty()) {
		敵がいなくなったらクリア

	}*/
	//プレイヤーのHPが0になったらゲームオーバー
	//レールカメラの移動が終わったらクリア
	if (sceneManager_->IsSceneAlive("FADE_OUT") == false && sceneManager_->IsSceneAlive("PLAYER_DEATH") == false) {
		if (!player_->GetIsAlive()) {
			sceneManager_->AddScene("PLAYER_DEATH");
		}

		if (railCamera_->GetIsFinished()) {
			sceneManager_->AddScene("FADE_OUT");
		}
	}
	if (sceneManager_->IsSceneFinished("FADE_OUT")) {
		if (!player_->GetIsAlive()) {
			sceneManager_->RemoveScene("PLAYER_DEATH");
			sceneManager_->AddScene("GAMEOVER");
		}
		if (railCamera_->GetIsFinished()) {
			sceneManager_->AddScene("CLEAR");
		}
		sceneManager_->RemoveScene("GAME");
		sceneManager_->RemoveScene("FADE_OUT");
		sceneManager_->AddScene("FADE_IN");
	}
}
