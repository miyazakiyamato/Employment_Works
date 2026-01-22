#include "GameOverUI.h"
#include "TextureManager.h"

void GameOverUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("gameOver.png");

	//スプライトの初期化
	std::unique_ptr<Sprite> sprite(new Sprite);
	sprite->Initialize("gameOver.png");
	sprite->SetPosition({ 640, 260 });
	sprite->SetSize({ 720.0f, 140.0f });
	sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_.push_back(std::move(sprite));
}

void GameOverUI::Update() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void GameOverUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}


