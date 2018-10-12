#pragma once
#include <System.h>
#include <glm/matrix.hpp>

class GameState {
public:
	virtual void Update(ECS::DeltaSeconds d) = 0;
	virtual void Draw() = 0;
	virtual const glm::mat4& GetView() const = 0;
};
