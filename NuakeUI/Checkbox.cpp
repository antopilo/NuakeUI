#include "Checkbox.h"

namespace NuakeUI
{
	Checkbox::Checkbox(const std::string& id)
	{
		ID = id;
		mNode = YGNodeNew();
		mType = NodeType::Checkbox;

		// Default Styling
		ComputedStyle.BorderSize = 8.f;
		SetWidthPixel(20.f); SetHeightPixel(20.f);
		ComputedStyle.BorderColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	}

	std::shared_ptr<Checkbox> Checkbox::New(const std::string& name)
	{
		return std::make_shared<Checkbox>(name);
	}

	void Checkbox::Tick()
	{

	}

	void Checkbox::UpdateInput(InputManager* inputManager)
	{
		float mx = inputManager->GetMouseX();
		float my = inputManager->GetMouseY();
		bool isHover = IsMouseHover(mx, my);
		if (isHover)
		{
			bool isMouseDown = inputManager->IsMouseInputDown();
			if (isMouseDown)
			{
				State = NodeState::Pressed;
			}
			else
			{
				if (State == NodeState::Pressed && !isMouseDown)
				{
					mChecked = !mChecked;
				}
				State = NodeState::Hover;
			}
		}
		else
		{
			State = NodeState::Idle;
		}

		for (auto& c : Childrens)
			c->UpdateInput(inputManager);
	}
}