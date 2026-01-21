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
#include <Easing.h>
#include "PlayerStateLeave.h"
#include <ChargeGun.h>
#include "SmallDrone.h"
#include "HpUI.h"
#include "OperationUI.h"
#include "GameSceneStateStart.h"

void GameScene::ChangeState(std::unique_ptr<BaseSceneState> newState) {
	state_ = std::move(newState);
	state_->Initialize(this);
}

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

	TextureManager::GetInstance()->LoadTexture("circle2.png");
	TextureManager::GetInstance()->LoadTexture("gradationLine.png");
	TextureManager::GetInstance()->LoadTexture("reticle.png");
	TextureManager::GetInstance()->LoadTexture("flash.png");
	TextureManager::GetInstance()->LoadTexture("rostock_laage_airport_4k.dds");

	//パーティクルシステムの生成
	particleSystem_.reset(new ParticleSystem);

	std::unique_ptr<EmitterSphere> emitterHit = std::make_unique<EmitterSphere>();
	emitterHit->Initialize("emitterHit", 100);
	emitterHit->SetTranslate({ 1.0f,1.0f,0.0f });
	emitterHit->SetTexture("flash.png");
	particleSystem_->SetParticleEmitter(std::move(emitterHit));

	std::unique_ptr<BaseParticleEmitter> hitEffect = std::make_unique<EmitterSphere>();
	hitEffect->Initialize("hitEffect", 100);
	hitEffect->SetPosition({ 1.0f,1.0f,0.0f });
	hitEffect->SetTexture("circle2.png");
	particleSystem_->SetParticleEmitter(std::move(hitEffect));

	std::unique_ptr<EmitterSphere> airEffect = std::make_unique<EmitterSphere>();
	airEffect->Initialize("airEffect", 1000);
	airEffect->SetTranslate({ 0.0f,0.0f,0.0f });
	airEffect->SetTexture("circle2.png");
	particleSystem_->SetParticleEmitter(std::move(airEffect));

	std::unique_ptr<EmitterSphere> chargeEffect = std::make_unique<EmitterSphere>();
	chargeEffect->Initialize("chargeEffect", 100);
	chargeEffect->SetTranslate({ 0.0f,0.0f,0.0f });
	chargeEffect->SetTexture("gradationLine.png");
	chargeEffect->SetRing(16, 0.5f, 0.0f);
	particleSystem_->SetParticleEmitter(std::move(chargeEffect));
	//バレットマネージャーの生成
	bulletManager_ = std::make_unique<BulletManager>();

	//ステージマネージャ
	stageManager_ = std::make_unique<StageManager>();
	stageManager_->Initialize(bulletManager_.get(), particleSystem_.get());
	player_ = stageManager_->GetPlayer();

	// Play Reticle UI
	std::unique_ptr<ReticleUI> reticleUI = std::make_unique<ReticleUI>();
	reticleUI->Initialize();
	player_->SetReticleUI(reticleUI.get());
	uiList_.push_back(std::move(reticleUI));

	// HP UI
	std::unique_ptr<HpUI> hpUI = std::make_unique<HpUI>();
	hpUI->Initialize(player_);
	uiList_.push_back(std::move(hpUI));

	// Operation UI
	std::unique_ptr<OperationUI> opUI = std::make_unique<OperationUI>();
	opUI->Initialize(player_);
	uiList_.push_back(std::move(opUI));

	ChangeState(std::make_unique<GameSceneStateStart>());
}

void GameScene::Finalize(){
	particleSystem_->Finalize();
	//解放

	bulletManager_->Finalize();
	stageManager_->Finalize();
	for (auto& ui : uiList_) {
		ui->Finalize();
	}
	uiList_.clear();
	BaseScene::Finalize();
}

void GameScene::Update() {
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

			particleSystem_->ImGuiUpdate();
			
			for (auto& ui : uiList_) {
				ui->ImGuiUpdate();
			}
			PostEffectManager::GetInstance()->ImGuiUpdate();
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
#endif //_DEBUG
	
	if (state_) {
		state_->Update();
	}
}

void GameScene::Draw(){
	if (state_) {
		state_->Draw();
	}
}


