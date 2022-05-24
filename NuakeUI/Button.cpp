#include "Button.h"


namespace NuakeUI
{
	std::shared_ptr<Button> Button::New(const std::string& name, const std::string& label)
	{
		return std::make_shared<Button>(name, label);
	}

	Button::Button(const std::string& name, const std::string& label) :
		Label(label)
	{
		mType = NodeType::Button;
		ID = name;
		mNode = YGNodeNew();
	}

	void Button::UpdateInput(InputManager* inputManager)
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
					// Calling the click callback.
					if (ClickCallback)
						ClickCallback();
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

	void Button::Tick()
	{
		
	}
}