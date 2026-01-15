#include "GameStartUI.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include <cmath>

void GameStartUI::Initialize(float duration) {
	duration_ = duration;
	spriteSize_ = { 180.0f, 144.0f }; 
	
	TextureManager::GetInstance()->LoadTexture("num.png");
	TextureManager::GetInstance()->LoadTexture("go.png");

	std::unique_ptr<Sprite> sprite(new Sprite);
	sprite->Initialize("num.png");
	sprite->SetPosition({ 640, 360 });
	sprite->SetSize(spriteSize_);
	sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprite->SetUVTranslate({ 0.3f, 0.0f });
	sprite->SetUVScale({ 0.1f, 1.0f });
	sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	sprites_.push_back(std::move(sprite));
}

void GameStartUI::Update() {
	counter_ += TimeManager::GetInstance()->kFlamTime_;

	if (!sprites_.empty()) {
		sprites_[0]->SetSize(spriteSize_ * counter_ * 2);
		if (counter_ < duration_) {
			float t = (counter_ / duration_) * 3.14159265f;
			float alpha = std::sinf(t);
			alpha = std::abs(alpha);
			sprites_[0]->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
		}
		else {
			counter_ = 0.0f;
			sprites_[0]->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			if (sprites_[0]->GetUVTranslate().x - 0.1f >= 0.1f) {
				sprites_[0]->SetUVTranslate({ sprites_[0]->GetUVTranslate().x - 0.1f, 0.0f });
			}
			else if (sprites_[0]->GetUVTranslate().x - 0.1f >= 0.0f) {
				sprites_[0]->SetTexture("go.png");
				sprites_[0]->SetUVTranslate({ 0.0f, 0.0f });
				spriteSize_ = { 180.0f, 144.0f };
				sprites_[0]->SetUVScale({ 1.0f, 1.0f });
				isFinished_ = true;
			}
			else {
				shouldClose_ = true;
			}
		}

		sprites_[0]->Update();
	}
}

void GameStartUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

void GameStartUI::Finalize() {
	sprites_.clear();
}
