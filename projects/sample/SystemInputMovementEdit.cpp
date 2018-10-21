#include "pch.h"

#include "SystemInputMovementEdit.h"

#include "Components.h"

void SystemInputMovementEdit::Run(ECS::EntityList& el, ECS::DeltaSeconds d)
{
	bool moveUp = m_InputState.MoveUpPressed();
	bool moveDown = m_InputState.MoveDownPressed();
	bool moveRight = m_InputState.MoveRightPressed();
	bool moveLeft = m_InputState.MoveLeftPressed();

	auto e = el.First<Component::LevelEditCursor, Component::Position>();

	if (e == nullptr)
	{
		SDL_Log("No cursor found");
		return;
	}

	Component::LevelEditCursor& cursor = e->Data<Component::LevelEditCursor>();
	Component::Position& pos = e->Data<Component::Position>();

	if (moveUp)
	{
		cursor.y -= 1;
	}

	if (moveDown)
	{
		cursor.y += 1;
	}

	if (moveLeft)
	{
		cursor.x -= 1;
	}

	if (moveRight)
	{
		cursor.x += 1;
	}

	if (cursor.y < 0)
	{
		cursor.y = 0;
	}

	if (cursor.x < 0)
	{
		cursor.x = 0;
	}

	pos.pos.x = static_cast<float>(cursor.x) + 0.5f;
	pos.pos.y = static_cast<float>(cursor.y + 1);
}
