#include "ClearUI.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include "Easing.h"

void ClearUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("clear.png");

	std::unique_ptr<Sprite> sprite(new Sprite);
	sprite->Initialize("clear.png");
	sprite->SetPosition({ 640, 260 });
	sprite->SetSize({ 360.0f, 140.0f });
	sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_.push_back(std::move(sprite));
}

void ClearUI::Update() {
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


