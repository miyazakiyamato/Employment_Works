#include "BaseCharacter.h"

void BaseCharacter::Initialize(){
	Collider::Initialize();
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
}

void BaseCharacter::Update(){
	object3d_->Update();
}

void BaseCharacter::ChangeState(std::unique_ptr<BaseCharacterState> newState) {
	state_ = std::move(newState);
	state_->Initialize();
}

Vector3 BaseCharacter::GetCenterPosition() const{
	return object3d_->GetCenterPosition();
}
