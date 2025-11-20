#include "TimedCall.h"
#include "TimeManager.h"

void TimedCall::Update() {
	if (IsFinish_) {
		return;
	}
	time_ -= TimeManager::GetInstance()->deltaTime_;
	if (time_ <= 0) {
		IsFinish_ = true;
		f_();
	}
}
