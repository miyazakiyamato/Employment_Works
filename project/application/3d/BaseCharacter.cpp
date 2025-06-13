#include "BaseCharacter.h"

void BaseCharacter::Initialize(){
	object3D_ = std::make_unique<Object3d>();
	object3D_->Initialize();
}

void BaseCharacter::Update(){
	object3D_->Update();
}

Vector3 BaseCharacter::GetCenterPosition() const
{
	return object3D_->GetCenterPosition();
}
