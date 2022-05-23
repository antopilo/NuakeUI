#pragma once

namespace NuakeUI
{
	class InputManager
	{
	public:
		virtual bool IsMouseInputDown() = 0;
		virtual float GetMouseX() = 0;
		virtual float GetMouseY() = 0;
	};
}