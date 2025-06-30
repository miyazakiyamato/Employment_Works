#include "GameScene.h"
#include <imgui.h>
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
	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("ground/ground.obj");

	/*ModelManager::GetInstance()->LoadModel("AnimatedCube/AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadAnimation("AnimatedCube/AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadModel("simpleSkin/simpleSkin.gltf");
	ModelManager::GetInstance()->LoadAnimation("simpleSkin/simpleSkin.gltf");
	ModelManager::GetInstance()->LoadModel("human/sneakWalk.gltf");
	ModelManager::GetInstance()->LoadAnimation("human/sneakWalk.gltf");
	ModelManager::GetInstance()->LoadModel("human/walk.gltf");
	ModelManager::GetInstance()->LoadAnimation("human/walk.gltf");*/

	TextureManager::GetInstance()->LoadTexture("circle2.png");
	TextureManager::GetInstance()->LoadTexture("gradationLine.png");
	TextureManager::GetInstance()->LoadTexture("reticle.png");

	//ParticleManager::GetInstance()->CreateParticleGroup();
	particleSystem_.reset(new ParticleSystem);
	std::unique_ptr<BaseParticleEmitter> hitEffect = std::make_unique<HitEffect>();
	particleSystem_->CreateParticleEmitter("hitEffect", std::move(hitEffect));

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
	//地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();
	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetGameScene(this);
	
	//エネミー
	for (uint32_t i = 0; i < 1; ++i) {
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->Initialize();
		enemy->SetParticleSystem(particleSystem_.get());
		enemies_.push_back(std::move(enemy));
	}
	//
	isAccelerationField = false;
	accelerationField_.reset(new AccelerationField);

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({ 0.0f, 1.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
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

	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy.reset();
	}
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet.reset();
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

	if (input_->TriggerKey(DIK_SPACE)) {
		AudioManager::GetInstance()->PlayWave("maou_se_system48.wav");
		//AudioManager::GetInstance()->PlayMP3("audiostock_1420737.mp3");
		//ParticleManager::GetInstance()->Emit("uvChecker", { 0,0,0 }, 10);
		//particleSystem_->Emit("hitEffect");
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
			groupName = "Camera";
			if (ImGui::BeginMenu(groupName.c_str())) {
				static int cameraItem_selected_idx = 0;
				globalVariables->ShowCombo(
					"Now Camera",
					{ "default", "Camera2","RailCamera"},
					cameraItem_selected_idx,
					[](const std::string& ItemName) { CameraManager::GetInstance()->FindCamera(ItemName); }
				);
				Vector3 cameraRotate = CameraManager::GetInstance()->GetCamera()->GetRotate();
				ImGui::DragFloat3("Camera.Rotate", &cameraRotate.x, 0.001f);

				Vector3 cameraPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
				ImGui::DragFloat3("Camera.Translate", &cameraPosition.x, 0.01f);
				
				CameraManager::GetInstance()->GetCamera()->SetRotate(cameraRotate);
				CameraManager::GetInstance()->GetCamera()->SetTranslate(cameraPosition);
				ImGui::EndMenu();
			}
			groupName = "Light";
			if (ImGui::BeginMenu(groupName.c_str())) {
				Vector4 DirectionalLightColor = LightManager::GetInstance()->GetDirectionalLight()->color;
				ImGui::ColorEdit4("DirectionalLight.Color", &DirectionalLightColor.x);
				Vector3 DirectionalLightDirection = LightManager::GetInstance()->GetDirectionalLight()->direction;
				ImGui::DragFloat3("DirectionalLight.Direction", &DirectionalLightDirection.x, 0.01f);
				DirectionalLightDirection = DirectionalLightDirection.Normalize();
				float DirectionalLightIntensity = LightManager::GetInstance()->GetDirectionalLight()->intensity;
				ImGui::DragFloat("DirectionalLight.Intensity", &DirectionalLightIntensity, 0.01f);
				bool IsBlinnPhong = (bool)LightManager::GetInstance()->GetDirectionalLight()->isBlinnPhong;
				ImGui::Checkbox("IsBlinnPhong", &IsBlinnPhong);

				int pointLightCount = LightManager::GetInstance()->GetDirectionalLight()->pointLightCount;
				ImGui::SliderInt("PointLightCount", &pointLightCount,0,10);
				int spotLightCount = LightManager::GetInstance()->GetDirectionalLight()->spotLightCount;
				ImGui::SliderInt("SpotLightCount", &spotLightCount,0,10);

				ImGui::Text("\n");
				LightManager::GetInstance()->SetDirectionalLight({ DirectionalLightColor,DirectionalLightDirection,DirectionalLightIntensity,(int)IsBlinnPhong,pointLightCount,spotLightCount });
				uint32_t idIndex = 0;
				for (uint32_t index = 0; index < LightManager::GetInstance()->GetPointLightCount(); index++) {
					ImGui::PushID(idIndex);
					Vector4 PointLightColor = LightManager::GetInstance()->GetPointLight()[index].color;
					ImGui::ColorEdit4("PointLight.Color", &PointLightColor.x);
					Vector3 PointLightPosition = LightManager::GetInstance()->GetPointLight()[index].position;
					ImGui::DragFloat3("PointLight.Position", &PointLightPosition.x, 0.01f);
					float PointLightIntensity = LightManager::GetInstance()->GetPointLight()[index].intensity;
					ImGui::DragFloat("PointLight.Intensity", &PointLightIntensity, 0.01f);
					float PointLightRadius = LightManager::GetInstance()->GetPointLight()[index].radius;
					ImGui::DragFloat("PointLight.Radius", &PointLightRadius, 0.01f);
					float PointLightDecay = LightManager::GetInstance()->GetPointLight()[index].decay;
					ImGui::DragFloat("PointLight.Decay", &PointLightDecay, 0.01f);

					LightManager::GetInstance()->SetPointLight(index, { PointLightColor,PointLightPosition,PointLightIntensity,PointLightRadius,PointLightDecay });
					ImGui::PopID();
					++idIndex;
				}
				ImGui::Text("\n");
				idIndex = 0;
				for (uint32_t index = 0; index < LightManager::GetInstance()->GetSpotLightCount(); index++) {
					ImGui::PushID(idIndex);
					Vector4 SpotLightColor = LightManager::GetInstance()->GetSpotLight()[index].color;
					ImGui::ColorEdit4("SpotLight.Color", &SpotLightColor.x);
					Vector3 SpotLightPosition = LightManager::GetInstance()->GetSpotLight()[index].position;
					ImGui::DragFloat3("SpotLight.Position", &SpotLightPosition.x, 0.01f);
					float SpotLightIntensity = LightManager::GetInstance()->GetSpotLight()[index].intensity;
					ImGui::DragFloat("SpotLight.Intensity", &SpotLightIntensity, 0.01f);
					Vector3 SpotLightDirection = LightManager::GetInstance()->GetSpotLight()[index].direction;
					ImGui::DragFloat3("SpotLight.Direction", &SpotLightDirection.x, 0.01f);
					SpotLightDirection = SpotLightDirection.Normalize();
					float SpotLightDistance = LightManager::GetInstance()->GetSpotLight()[index].distance;
					ImGui::DragFloat("SpotLight.Distance", &SpotLightDistance, 0.01f);
					float SpotLightDecay = LightManager::GetInstance()->GetSpotLight()[index].decay;
					ImGui::DragFloat("SpotLight.Decay", &SpotLightDecay, 0.01f);
					float SpotLightCosAngle = LightManager::GetInstance()->GetSpotLight()[index].cosAngle;
					ImGui::DragFloat("SpotLight.CosAngle", &SpotLightCosAngle, 0.01f);
					float SpotLightCosFalloffStart = LightManager::GetInstance()->GetSpotLight()[index].cosFalloffStart;
					ImGui::DragFloat("SpotLight.CosFalloff", &SpotLightCosFalloffStart, 0.01f);

					LightManager::GetInstance()->SetSpotLight(index,{ SpotLightColor,SpotLightPosition,SpotLightIntensity,SpotLightDirection,SpotLightDistance,SpotLightDecay,SpotLightCosAngle,SpotLightCosFalloffStart });
					ImGui::PopID();
					++idIndex;
				}
				ImGui::EndMenu();
			}
			std::string blendName = "Now Blend";
			std::vector<std::string> blendState{
				"None",      //!< ブレンドなし
				"Normal",    //!< 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
				"Add",       //!< 加算。Src * SrcA + Dest * 1
				"Subtract",  //!< 減算。Dest * 1 - Src * SrcA
				"Multiply",  //!< 乗算。Src * 0 + Dest * Src
				"Screen", };
			/*groupName = "Object3d";
			if (ImGui::BeginMenu(groupName.c_str())) {
				
				size_t object3dCount = 0;
				for (std::unique_ptr<Object3d>& object3d : object3ds_) {
					int Object3dItem_selected_idx = static_cast<int>(object3d->GetBlendMode());
					const char* currentItem = blendState[Object3dItem_selected_idx].c_str();
					if (ImGui::BeginCombo((blendName + std::to_string(object3dCount)).c_str(), currentItem)) {
						for (int i = 0; i < blendState.size(); ++i) {
							bool isSelected = (Object3dItem_selected_idx == i);
							if (ImGui::Selectable(blendState[i].c_str(), isSelected)) {
								Object3dItem_selected_idx = i;
								object3d->SetBlendMode(static_cast<BlendMode>(i));
							}
							if (isSelected) {
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					Vector3 translate = object3d->GetTranslate();
					ImGui::DragFloat3(("Object3d " + std::to_string(object3dCount) + ".Transform.Translate").c_str(), &translate.x, 0.1f);

					Vector3 rotate = object3d->GetRotate();
					ImGui::SliderAngle(("Object3d " + std::to_string(object3dCount) + ".Transform.Rotate.x").c_str(), &rotate.x);
					ImGui::SliderAngle(("Object3d " + std::to_string(object3dCount) + ".Transform.Rotate.y").c_str(), &rotate.y);
					ImGui::SliderAngle(("Object3d " + std::to_string(object3dCount) + ".Transform.Rotate.z").c_str(), &rotate.z);

					Vector3 scale = object3d->GetScale();
					ImGui::DragFloat3(("Object3d " + std::to_string(object3dCount) + ".Transform.Scale").c_str(), &scale.x, 0.1f);

					Vector4 color = object3d->GetColor();
					ImGui::ColorEdit4(("Object3d " + std::to_string(object3dCount) + ".Color").c_str(), &color.x);

					Vector4 highLightColor = object3d->GetHighLightColor();
					ImGui::ColorEdit4(("Object3d " + std::to_string(object3dCount) + ".HighLightColor").c_str(), &highLightColor.x);

					object3d->SetTranslate(translate);
					object3d->SetRotate(rotate);
					object3d->SetScale(scale);
					object3d->SetColor(color);
					object3d->SetHighLightColor(highLightColor);
					
					ImGui::Text("\n");
					object3dCount++;
				}
				ImGui::EndMenu();
			}*/
			ParticleManager::GetInstance()->UpdateGlobalVariables();
			groupName = "Particle";
			//if (ImGui::BeginMenu(groupName.c_str())) {
			//	int particleItem_selected_idx = static_cast<int>(ParticleManager::GetInstance()->GetBlendMode(particleEmitter_->GetName()));
			//	const char* currentItem = blendState[particleItem_selected_idx].c_str();
			//	if (ImGui::BeginCombo((blendName + particleEmitter_->GetName()).c_str(), currentItem)) {
			//		for (int i = 0; i < blendState.size(); ++i) {
			//			bool isSelected = (particleItem_selected_idx == i);
			//			if (ImGui::Selectable(blendState[i].c_str(), isSelected)) {
			//				particleItem_selected_idx = i;
			//				ParticleManager::GetInstance()->SetBlendMode(particleEmitter_->GetName(),static_cast<BlendMode>(i));
			//			}
			//			if (isSelected) {
			//				ImGui::SetItemDefaultFocus();
			//			}
			//		}
			//		ImGui::EndCombo();
			//	}

			//	Vector3 position = particleEmitter_->GetPosition();
			//	ImGui::DragFloat2("particleEmitter_.Translate", &position.x, 0.1f);
			//	/*if (position.y > 640.0f) {
			//		position.y = 640.0f;
			//	}*/

			//	//Vector3 rotation = particleEmitter_->GetRotation();
			//	//ImGui::SliderAngle("particleEmitter_.Rotate", &rotation.x);

			//	//Vector3 size = particleEmitter_->GetSize();
			//	//ImGui::DragFloat2("particleEmitter_.Scale", &size.x, 0.1f);
			//	//if (size.y > 360.0f) {
			//	//	size.y = 360.0f;
			//	//}

			//	int count = particleEmitter_->GetCount();
			//	ImGui::DragInt("particleEmitter_.count", &count, 1, 0, 1000);

			//	float frequency = particleEmitter_->GetFrequency();
			//	ImGui::DragFloat("particleEmitter_.frequency", &frequency, 0.1f);

			//	if (ImGui::Button("ParticleEmit", { 100,50 })) {
			//		particleEmitter_->Emit();
			//	}

			//	bool isEmitUpdate = particleEmitter_->GetIsEmitUpdate();
			//	ImGui::Checkbox("IsEmitUpdate", &isEmitUpdate);

			//	particleEmitter_->SetPosition(position);
			//	//particleEmitter_->SetRotation(rotation);
			//	//particleEmitter_->SetSize(size);
			//	particleEmitter_->SetCount(count);
			//	particleEmitter_->SetFrequency(frequency);
			//	particleEmitter_->SetIsEmitUpdate(isEmitUpdate);
			//	
			//	ImGui::Checkbox("IsAccelerationField", &isAccelerationField);
			//	ImGui::EndMenu();
			//}
			groupName = "Sprite";
			if (ImGui::BeginMenu(groupName.c_str())) {
				uint32_t objectIDIndex = 0;
				for (std::unique_ptr<Sprite>& sprite : sprites_) {
					ImGui::PushID(objectIDIndex);
					int SpriteItem_selected_idx = static_cast<int>(sprite->GetBlendMode());
					const char* currentItem = blendState[SpriteItem_selected_idx].c_str();
					if (ImGui::BeginCombo((blendName + std::to_string(objectIDIndex)).c_str(), currentItem)) {
						for (int i = 0; i < blendState.size(); ++i) {
							bool isSelected = (SpriteItem_selected_idx == i);
							if (ImGui::Selectable(blendState[i].c_str(), isSelected)) {
								SpriteItem_selected_idx = i;
								sprite->SetBlendMode(static_cast<BlendMode>(i));
							}
							if (isSelected) {
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}


					Vector2 position = sprite->GetPosition();
					ImGui::DragFloat2("Sprite.Translate", &position.x, 1.0f, 0.0f, 1180.0f, "%.1f");
					/*if (position.y > 640.0f) {
						position.y = 640.0f;
					}*/

					float rotation = sprite->GetRotation();
					ImGui::SliderAngle("Sprite.Rotate", &rotation);

					Vector2 size = sprite->GetSize();
					ImGui::DragFloat2("Sprite.Scale", &size.x, 1.0f, 0.0f, 640.0f, "%.1f");
					if (size.y > 360.0f) {
						size.y = 360.0f;
					}

					Vector4 color = sprite->GetColor();
					ImGui::ColorEdit4("Sprite.Color", &color.x);

					sprite->SetPosition(position);
					sprite->SetRotation(rotation);
					sprite->SetSize(size);
					sprite->SetColor(color);
					
					ImGui::Text("\n");
					ImGui::PopID();
					++objectIDIndex;
				}
				ImGui::EndMenu();
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

	//当たり判定
	CheckAllCollisions();

	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Update();
	}
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->isAlive()) {
			bullet.reset();
			return true;
		}
		return false;
		});
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
}

void GameScene::Draw(){
	//Object3dの描画
	//天球
	skydome_->Draw();
	//地面
	ground_->Draw();
	//プレイヤー
	player_->Draw();
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Draw();
	}

	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw();
	}
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

void GameScene::AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet){
	if (playerBullet) {
		playerBullets_.push_back(std::move(playerBullet));
	} 
}

void GameScene::CheckAllCollisions(){
	//衝突マネージャのリストクリアする
	collisionManager_->Reset();
	//全てのコライダーを衝突マネージャのリストに登録する
	collisionManager_->AddCollider(player_.get());
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		collisionManager_->AddCollider(bullet.get());
	}
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
	}
	//リスト内の総当たり判定
	collisionManager_->CheckAllCollisions();
}
