#pragma once

namespace NuakeUI
{
	class InputManager
	{
	public:
		static float ScrollX;
		static float ScrollY;

		virtual bool IsMouseInputDown() = 0;
		virtual float GetMouseX() = 0;
		virtual float GetMouseY() = 0;

		virtual float GetScrollX() = 0;
		virtual float GetScrollY() = 0;
	};
}