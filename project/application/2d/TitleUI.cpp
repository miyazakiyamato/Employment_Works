#include "TitleUI.h"
#include "TextureManager.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif
#include <string>
#include <cmath>
#include "TimeManager.h"

namespace Engine {

void TitleUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("title.dds");
	TextureManager::GetInstance()->LoadTexture("AStart.dds");

	//スプライトの初期化
	for (uint32_t i = 0; i < 2; ++i) {
		std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
		sprite->SetAnchorPoint({ 0.5f, 0.5f });
		sprites_.push_back(std::move(sprite));
	}
	sprites_[0]->Initialize("title.dds");
	sprites_[0]->SetPosition({ 640, 260 });
	sprites_[0]->SetSize({ 800.0f, 300.0f });
	
	sprites_[1]->Initialize("AStart.dds");
	sprites_[1]->SetPosition({ 640, 460 });
	sprites_[1]->SetSize({ 144.0f, 50.0f });
}

void TitleUI::Update() {
	animationTime_ += TimeManager::GetInstance()->deltaTime_;
	float alpha = (std::sin(animationTime_ * 2.0f) + 1.0f) / 2.0f; // 0.0 to 1.0 sine wave
	
	// ゆっくり点滅させるため、範囲を調整 (例: 0.2 ~ 1.0)
	//alpha = 0.2f + (alpha * 0.8f);

	sprites_[1]->SetColor({ 1.0f,1.0f,1.0f, alpha });

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void TitleUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

void TitleUI::ImGuiUpdate() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("TitleUI")) {
		int i = 0;
		for (std::unique_ptr<Sprite>& sprite : sprites_) {
			std::string name = "TitleSprite" + std::to_string(i++);
			sprite->ImGuiUpdate(name);
		}
		ImGui::TreePop();
	}
#endif
}

} // namespace Engine
