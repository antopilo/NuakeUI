#pragma once
#include <NuakeRenderer/Math.h>

#include "NodeState.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace NuakeUI
{
	enum class NodeState;
	enum class StyleProperties {
		Width, Height, MinWidth, MinHeight, MaxWidth, MaxHeight,
		PaddingLeft, PaddingTop, PaddingRight, PaddingBottom,
		MarginLeft, MarginTop, MarginRight, MarginBottom, 
		Position, AlignItems, SelfAlign,
		AspectRatio, FlexDirection, FlexWrap, FlexBasis, FlexGrow, FlexShrink,
		JustifyContent, AlignContent, LayoutDirection, 
		BorderSize, BorderRadius, BorderColor,
		BackgroundColor, TextAlign, Color, Overflow
	};
	enum class PositionType { Relative, Absolute };
	enum class AlignItemsType { Auto, FlexStart, Center, FlexEnd, Stretch, Baseline, SpaceBetween, SpaceAround };
	enum class FlexWrapType { NoWrap, Wrap, WrapReversed };
	enum class JustifyContentType { FlexStart, FlexEnd, Center, SpaceBetween, SpaceAround, SpaceEvenly };
	enum class FlexDirectionType { Row, Column, RowReversed, ColumnReversed };
	enum class AlignContentType { FlexStart, FlexEnd, Stretch, Center, SpaceBetween, SpaceAround };
	enum class LayoutDirectionType { LTR = 2, RTL = 3 };

	enum class TextAlignType
	{
		Left, Center, Right
	};

	// Style Values
	enum class PropValueType { Pixel , Percent, Auto, Color, Enum };

	union Value
	{
		float Number;
		Color Color;
		int Enum;
	};

	struct PropValue
	{
		PropValueType type;
		Value value;
	};

	// Selector
	enum class StyleSelectorType { Id, Class, Tag, Pseudo };

	struct StyleSelector
	{
		StyleSelectorType Type;
		std::string Value;
	};

	class StyleRule
	{
	public:
		std::vector<StyleSelector> Selector;

		std::map<StyleProperties, PropValue> Properties;

		StyleRule(std::vector<StyleSelector> selector) : Selector(selector)
		{
			Properties = std::map<StyleProperties, PropValue>();
		}

		void SetProp(StyleProperties prop, PropValue value)
		{
			Properties[prop] = value;
		}

		PropValue& GetProp(StyleProperties prop)
		{
			if (HasProp(prop))
				return Properties[prop];

			return PropValue();
		}

		bool HasProp(StyleProperties prop)
		{
			return Properties.find(prop) != Properties.end();
		}
	};

	class StyleSheet
	{
	public:
		std::vector<StyleRule> Rules;

		static std::shared_ptr<StyleSheet> New()
		{
			return std::make_shared<StyleSheet>();
		}

		StyleSheet() = default;
		~StyleSheet() = default;
	};
}