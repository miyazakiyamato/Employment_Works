#include "TitleUI.h"
#include "TextureManager.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif
#include <string>

void TitleUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("title.png");
	TextureManager::GetInstance()->LoadTexture("AStart.png");

	//スプライトの初期化
	for (uint32_t i = 0; i < 2; ++i) {
		std::unique_ptr<Sprite> sprite(new Sprite);
		sprite->SetAnchorPoint({ 0.5f, 0.5f });
		sprites_.push_back(std::move(sprite));
	}
	sprites_[0]->Initialize("title.png");
	sprites_[0]->SetPosition({ 640, 260 });
	sprites_[0]->SetSize({ 800.0f, 300.0f });
	
	sprites_[1]->Initialize("AStart.png");
	sprites_[1]->SetPosition({ 640, 460 });
	sprites_[1]->SetSize({ 250.0f, 100.0f });
}

void TitleUI::Update() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void TitleUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

void TitleUI::Finalize() {
	sprites_.clear();
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
