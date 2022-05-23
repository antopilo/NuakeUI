#pragma once

namespace NuakeUI
{
	enum class LengthUnit
	{
		Pixel, Percentage, Constraint
	};

	struct Length
	{
		LengthUnit Unit;
		float Value;
	};

	struct Margin
	{
		float Bottom;
		float Left;
		float Right;
		float Top;
	};

	enum class Styles
	{
		Width, Height, Margin, Padding
	};


	enum class ValueType
	{
		Float, Length, Color,

	};

	struct StyleValue
	{

	};
}
