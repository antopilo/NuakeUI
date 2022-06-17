#pragma once
#include <NuakeRenderer/Math.h>
#include "Style.h"

namespace NuakeUI
{
	enum class LengthType
	{
		Percentage, Pixel, Auto
	};

	struct Length
	{
		float value = 1.f;
		LengthType type = LengthType::Auto;
	};

	struct NodeStyle
	{
		Color BackgroundColor = Color(0, 0, 0, 0);
		float BorderWidth = 0.f;
		Length Width;
		Length MinWidth;
		Length MaxWidth;
		Length Height;
		Length MinHeight;
		Length MaxHeight;
		Length PaddingLeft = { 0.f, LengthType::Pixel };
		Length PaddingTop = { 0.f, LengthType::Pixel };
		Length PaddingRight = { 0.f, LengthType::Pixel };
		Length PaddingBottom = { 0.f, LengthType::Pixel };
		Length MarginLeft = { 0.f, LengthType::Pixel };
		Length MarginTop = { 0.f, LengthType::Pixel };
		Length MarginRight = { 0.f, LengthType::Pixel };
		Length MarginBottom = { 0.f, LengthType::Pixel };
		PositionType Position = PositionType::Relative;
		AlignItemsType SelfAlign;
		AlignItemsType AlignItems;
		float AspectRatio;
		FlexDirectionType FlexDirection;
		FlexWrapType FlexWrap;
		float FlexBasis;
		float FlexGrow;
		float FlexShrink;
		JustifyContentType JustifyContent;
		AlignContentType AlignContent;
		LayoutDirectionType LayoutDirection;
		float BorderSize = 0.f;
		float BorderRadius = 0.f;
		Color BorderColor = Color(0, 0, 0, 0);
		float FontSize = 64.0f;
		TextAlignType TextAlign = TextAlignType::Left;
		Color FontColor = Color(1, 1, 1, 1);
		OverflowType Overflow = OverflowType::Show;
		VisibilityType Visibility = VisibilityType::Show;
	};
}