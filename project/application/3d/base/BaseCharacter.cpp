#include "BaseCharacter.h"

void BaseCharacter::Initialize(){
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
}

void BaseCharacter::Update(){
	object3d_->Update();
}

Vector3 BaseCharacter::GetCenterPosition() const
{
	return object3d_->GetCenterPosition();
}
