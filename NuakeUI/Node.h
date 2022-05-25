#pragma once
#include "InputManager.h"

#include <NuakeRenderer/Math.h>
#include <yoga/yoga.h>

#include <map>
#include <vector>
#include <string>
#include "StyleSheet.h"


#define SetLength(name) \
if (ComputedStyle.##name.type == LengthType::Auto)  \
YGNodeStyleSet##name##Auto(mNode); \
else if (ComputedStyle.##name.type == LengthType::Pixel) YGNodeStyleSet##name(mNode, ComputedStyle.##name.value); \
else if (ComputedStyle.##name.type == LengthType::Percentage) YGNodeStyleSet##name##Percent(mNode, ComputedStyle.##name.value); \

#define SetLengthNoAuto(name) \
if (ComputedStyle.##name.type == LengthType::Pixel) YGNodeStyleSet##name(mNode, ComputedStyle.##name.value); \
else if (ComputedStyle.##name.type == LengthType::Percentage) YGNodeStyleSet##name##Percent(mNode, ComputedStyle.##name.value); \



#define LengthProp(name)  \
				case StyleProperties::name: \
				{ \
					switch (value.type) \
					{ \
					case PropValueType::Pixel: \
					{ \
						ComputedStyle.name.type = LengthType::Pixel; \
						ComputedStyle.name.value = value.value.Number; \
					} \
					break; \
					case PropValueType::Percent: \
					{ \
						ComputedStyle.name.type = LengthType::Percentage;  \
						ComputedStyle.name.value = value.value.Number; \
					} \
					break; \
					case PropValueType::Auto: \
					{ \
						ComputedStyle.name.type = LengthType::Auto; \
					} \
					break; \
					} \
				} \
break; \


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
		Length PaddingLeft;
		Length PaddingTop;
		Length PaddingRight;
		Length PaddingBottom;
		Length MarginLeft;
		Length MarginTop;
		Length MarginRight;
		Length MarginBottom;
		PositionType Position;
		AlignItemsType SelfAlign;
		AlignItemsType AlignItems;
		float Ratio;
		FlexDirectionType FlexDirection;
		FlexWrapType FlexWrap;
		float FlexBasis;
		float FlexGrow;
		float FlexShrink;
		JustifyContentType JustifyContent;
		AlignContentType AlignContent;
		LayoutDirectionType LayoutDirection;
		float BorderSize = 0.f;
		float BorderRadius;
		Color BorderColor = Color(0, 0, 0, 0);
	};

	enum class State
	{
		Idle, Hover, Pressed, Clicked
	};

	enum class NodeType
	{
		Node, Button, Checkbox, Text
	};

	class Node
	{
	protected:
		std::string ID = ""; // Name of the node in the tree
		YGNodeRef mNode; // Yoga_CPP nodes.
		std::vector<std::shared_ptr<Node>> Childrens = std::vector<std::shared_ptr<Node>>();

		NodeType mType = NodeType::Node;
		State mState = State::Idle;
	public:
		NodeStyle ComputedStyle; // The current visual styles.
		
		std::vector<std::string> Classes = std::vector<std::string>(); // List of css classes.

		// Should be used to creates nodes since it creates a shared_ptr for you.
		static std::shared_ptr<Node> New(const std::string id);

		Node(const std::string& id); // Do not use.
		Node() = default;
		~Node() = default;

		inline NodeType GetType() const { return mType; }
	public:
		virtual void Draw(int z);
		virtual void UpdateInput(InputManager* manager);
		virtual void Tick();

		void ApplyStyleProperties(std::map<StyleProperties, PropValue> properties);

		// Calculates the layout.
		virtual void Calculate();

		bool IsMouseHover(float x, float y);
		State GetState() const { return mState; }

		YGNodeRef GetYogaNode() const { return mNode; }
		std::string GetID() const { return ID; }

		// Childrens
		std::vector<std::shared_ptr<Node>> GetChildrens() const;

		void InsertChild(std::shared_ptr<Node> child);

		template<class T>
		std::shared_ptr<T> GetChild(unsigned int index)
		{
			assert(index < Childrens.size()); // No childrens.
			return std::static_pointer_cast<T>(Childrens[index]);
		}

		template<class T>
		std::shared_ptr<T> GetChildByID(const std::string& id)
		{
			assert(Childrens.size() > 0); // No childrens.
			for (auto& c : Childrens)
			{
				if (c->ID == id)
					return std::static_pointer_cast<T>(c);
			}

			assert(false);  // Node not found.
		}

		template<class T>
		std::shared_ptr<T> FindChildByID(const std::string& id)
		{
			for (auto& c : Childrens)
			{
				if (c->ID == id)
					return std::static_pointer_cast<T>(c);

				auto result = c->FindChildByID<T>(id);
				if (result != nullptr)
					return result;
			}

			return nullptr;
		}

		void SetHeightPixel(float h)  const { YGNodeStyleSetHeight(mNode, h); }
		void SetWidthPixel(float w)    const { YGNodeStyleSetWidth(mNode, w); }
		void SetHeightPercent(float h) const { YGNodeStyleSetHeightPercent(mNode, h); }
		void SetWidthPercent(float w)  const { YGNodeStyleSetWidthPercent(mNode, w); }
		void SetMaxHeightPixel(float h) const { YGNodeStyleSetMaxHeight(mNode, h); }
		void SetMaxHeightPercent(float h) const { YGNodeStyleSetMaxHeightPercent(mNode, h); }
		void SetMaxWidthPixel(float h) const { YGNodeStyleSetMaxWidth(mNode, h); }
		void SetMaxWidthPercent(float h) const { YGNodeStyleSetMaxWidthPercent(mNode, h); }
		void SetDisplay(YGDisplay display) const { YGNodeStyleSetDisplay(mNode, display); }
		void SetJustify(YGJustify justify) const { YGNodeStyleSetJustifyContent(mNode, justify); }
		void SetAlignItem(YGAlign align) const { YGNodeStyleSetAlignItems(mNode, align); }
		void SetBorder(float w)  { ComputedStyle.BorderSize = w; }
		void SetBorderColor(Color c) { ComputedStyle.BorderColor = c; }
	};
}