#include "GameOverUI.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include <cmath>

namespace Engine {

void GameOverUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("gameOver.dds");
	TextureManager::GetInstance()->LoadTexture("ATitle.dds");

	//スプライトの初期化
	// game over sprite
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize("gameOver.dds");
	sprite->SetPosition({ 640, 260 });
	sprite->SetSize({ 720.0f, 140.0f });
	sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_.push_back(std::move(sprite));

	// ATitle sprite
	std::unique_ptr<Sprite> spriteTitle = std::make_unique<Sprite>();
	spriteTitle->Initialize("ATitle.dds");
	spriteTitle->SetPosition({ 640, 460 });
	spriteTitle->SetSize({ 160.0f, 50.0f });
	spriteTitle->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_.push_back(std::move(spriteTitle));
}

void GameOverUI::Update() {
	animationTime_ += TimeManager::GetInstance()->deltaTime_;
	float alpha = (std::sin(animationTime_ * 2.0f) + 1.0f) / 2.0f;
	
	if (sprites_.size() > 1) {
		sprites_[1]->SetColor({ 1.0f,1.0f,1.0f, alpha });
	}

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void GameOverUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

} // namespace Engine
