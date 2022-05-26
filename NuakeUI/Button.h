#pragma once
#include "Node.h"
#include "InputManager.h"

#include <string>

namespace NuakeUI
{
	class Button : public Node
	{
	private:
		bool mHasBeenClicked = false;
		bool mHasCallback = false;
	public:
		std::string Label = "";

		Button(const std::string& name, const std::string& label);
		~Button() {};

		static std::shared_ptr<Button> New(const std::string& name, const std::string& label);

		void (*ClickCallback)(NuakeUI::Button&);

		void Draw();
		void Tick() override;
		void UpdateInput(InputManager* inputManager) override;

		void SetClickCallback(void(*callback)(NuakeUI::Button&))
		{
			mHasCallback = true;
			ClickCallback = callback;
		}
	};
}