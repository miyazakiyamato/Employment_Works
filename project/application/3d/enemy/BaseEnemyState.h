#pragma once
#include <string>

class BaseEnemy;

class BaseEnemyState {
public:
	BaseEnemyState(const std::string& name, BaseEnemy* enemy) : name_(name), enemy_(enemy){};

	virtual ~BaseEnemyState() {}

	virtual void Initialize() = 0;
	virtual void Update() = 0;

	virtual void DebugLog();

protected:
	std::string name_;
	   
	BaseEnemy* enemy_ = nullptr;
};
