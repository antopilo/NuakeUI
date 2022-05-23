#pragma once
#include <string>
#include "Node.h"

namespace NuakeUI
{
	enum class ButtonState
	{
		Normal, Hover, Pressed
	};

	class Button : public Node
	{
	public:
		ButtonState State = ButtonState::Normal;

		std::string Label = "";

		Button(const std::string& name, const std::string& label);
		~Button() {};

		static std::shared_ptr<Button> New(const std::string& name, const std::string& label);

		void (*ClickCallback)();

		void Draw();
		void Tick();

		void SetClickCallback(void(*callback)())
		{
			ClickCallback = callback;
		}
	};
}