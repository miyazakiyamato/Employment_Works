#include "PlayerDeathUI.h"
#include "TextureManager.h"

void PlayerDeathUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("num.png");

	std::unique_ptr<Sprite> sprite(new Sprite);
	sprite->Initialize("num.png");
	sprite->SetPosition({ 640, 360 });
	sprite->SetSize({ 90.0f, 144.0f });
	sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprite->SetUVTranslate({ 0.3f, 0.0f });
	sprite->SetUVScale({ 0.1f, 1.0f });
	sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	sprites_.push_back(std::move(sprite));
}

void PlayerDeathUI::Update() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void PlayerDeathUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		//sprite->Draw();
	}
}

void PlayerDeathUI::SetSpriteSize(const Vector2& size) {
	if (!sprites_.empty()) {
		sprites_[0]->SetSize(size);
	}
}

void PlayerDeathUI::SetColor(const Vector4& color) {
	if (!sprites_.empty()) {
		sprites_[0]->SetColor(color);
	}
}
