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
	public:
		std::string Label = "";

		Button(const std::string& name, const std::string& label);
		~Button() {};

		static std::shared_ptr<Button> New(const std::string& name, const std::string& label);

		void (*ClickCallback)();

		void Draw();
		void Tick() override;
		void UpdateInput(InputManager* inputManager) override;

		void SetClickCallback(void(*callback)())
		{
			ClickCallback = callback;
		}
	};
}