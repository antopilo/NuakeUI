#include "Checkbox.h"

namespace NuakeUI
{
	Checkbox::Checkbox(const std::string& id)
	{
		ID = id;
		mNode = YGNodeNew();

		// Default Styling
		Style.border = 8.f;
		SetWidthPixel(20.f); SetHeightPixel(20.f);
		Style.border_color = { 0.1f, 0.1f, 0.1f, 1.0f };
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
				mState = State::Pressed;
			}
			else
			{
				if (mState == State::Pressed && !isMouseDown)
				{
					mChecked = !mChecked;
				}
				mState = State::Hover;
			}
		}
		else
		{
			mState = State::Idle;
		}

		for (auto& c : Childrens)
			c->UpdateInput(inputManager);
	}
}