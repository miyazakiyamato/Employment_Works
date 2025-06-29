#include "Skydome.h"
#include "TextureManager.h"

void Skydome::Initialize(){
	object3d = std::make_unique<Object3d>();
	object3d->Initialize();
	object3d->SetModel("skydome/skydome.obj");
	//object3d->SetIsLighting(false);
	object3d->SetTranslate({ -1,0,0 });
	object3d->SetRotate({ 0,3.14f,0 });
	object3d->SetScale({ 10,10,10 });
	std::string objectName = "skydome";
}

void Skydome::Update(){
	object3d->Update();
}

void Skydome::Draw(){
	object3d->Draw();
}
