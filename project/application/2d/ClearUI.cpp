#include "ClearUI.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include "Easing.h"
#include <cmath>

void ClearUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("clear.dds");
	TextureManager::GetInstance()->LoadTexture("ATitle.dds");

	std::unique_ptr<Sprite> sprite(new Sprite);
	sprite->Initialize("clear.dds");
	sprite->SetPosition({ 640, 260 });
	sprite->SetSize({ 360.0f, 140.0f });
	sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_.push_back(std::move(sprite));

	// ATitle sprite
	std::unique_ptr<Sprite> spriteTitle(new Sprite);
	spriteTitle->Initialize("ATitle.dds");
	spriteTitle->SetPosition({ 640, 460 });
	spriteTitle->SetSize({ 160.0f, 50.0f });
	spriteTitle->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_.push_back(std::move(spriteTitle));
}

void ClearUI::Update() {
	animationTime_ += TimeManager::GetInstance()->deltaTime_;
	float alpha = (std::sin(animationTime_ * 2.0f) + 1.0f) / 2.0f;

	if (sprites_.size() > 1) {
		sprites_[1]->SetColor({ 1.0f,1.0f,1.0f, alpha });
		sprites_[1]->Update();
	}

	scaleCount_ += TimeManager::GetInstance()->deltaTime_;
	if (scaleCount_ > 1.0f) {
		scaleCount_ = 0.0f;
	}
	float scale{};
	if (scaleCount_ <= 0.5f) {
		scale = Easing::EaseInOutSine(scaleCount_ * 2.0f, 0.5f, 1.0f);
	}
	else {
		scale = Easing::EaseInOutSine(scaleCount_ * 2.0f - 1.0f, 1.0f, 0.5f);
	}
	
	if (!sprites_.empty()) {
		sprites_[0]->SetSize(Vector2(360.0f, 140.0f) * scale);
		sprites_[0]->Update();
	}
}

void ClearUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}
