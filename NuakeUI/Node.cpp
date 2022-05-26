#include "Node.h"
#include "Renderer.h"

#include "NodeState.h"

namespace NuakeUI
{
	std::shared_ptr<Node> Node::New(const std::string id)
	{
		return std::make_shared<Node>(id);
	}

	Node::Node(const std::string& id) : ID(id)
	{
		assert(id != "");
		mNode = YGNodeNew();
		Childrens = std::vector<std::shared_ptr<Node>>();

		YGNodeStyleSetWidthAuto(mNode);
		YGNodeStyleSetWidthAuto(mNode);
	}

	void Node::Tick()
	{
		// Update the state.
		for (auto& c : Childrens)
			c->Tick();
	}

	void Node::UpdateInput(InputManager* inputManager)
	{
		float mx = inputManager->GetMouseX();
		float my = inputManager->GetMouseY();

		bool isHover = IsMouseHover(mx, my);
		if (!isHover)
			State = NodeState::Idle;
		else
			State = NodeState::Hover;

		if (isHover && inputManager->IsMouseInputDown())
			State = NodeState::Clicked;

		for (auto& c : Childrens)
			c->UpdateInput(inputManager);
	}

	void Node::Draw(int z)
	{
		z += 1;
		for (auto& c : Childrens)
		{
			Renderer::Get().DrawNode(c, z);
			c->Draw(z );
		}
	}

	void Node::Calculate()
	{
		for (auto& c : Childrens)
			c->Calculate();
	}

	bool Node::IsMouseHover(float x, float y)
	{
		YGNodeRef ygNode = GetYogaNode();
		float width   = YGNodeLayoutGetWidth(ygNode);
		float height  = YGNodeLayoutGetHeight(ygNode);
		float padding = YGNodeLayoutGetPadding(ygNode, YGEdgeLeft);
		float left    = YGNodeLayoutGetLeft(ygNode);
		float top = YGNodeLayoutGetTop(ygNode);

		float parentLeft = 0.0f;
		float parentTop = 0.0f;
		for (YGNodeRef parent = YGNodeGetParent(ygNode); parent; parent = YGNodeGetParent(parent))
		{
			parentLeft += YGNodeLayoutGetLeft(parent);
			parentTop += YGNodeLayoutGetTop(parent);
		}

		left += parentLeft;
		top += parentTop;
		bool isHover = x > left && x < left + width && y > top && y < top + height;
		return isHover;
	}

	void Node::InsertChild(std::shared_ptr<Node> child)
	{
		child->Parent = this;
		Childrens.push_back(child);
		YGNodeInsertChild(this->mNode, child->GetYogaNode(), Childrens.size() - 1);
	}

	void Node::ApplyStyleProperties(std::map<StyleProperties, PropValue> properties)
	{
		for (auto& p : properties)
		{
			StyleProperties prop = p.first;
			PropValue value = p.second;

			switch (prop)
			{
				LengthProp(Width)
				LengthProp(Height)
				LengthProp(MinHeight)
				LengthProp(MinWidth)
				LengthProp(MaxHeight)
				LengthProp(MaxWidth)
				LengthProp(MarginLeft)
				LengthProp(MarginTop)
				LengthProp(MarginRight)
				LengthProp(MarginBottom)
				LengthProp(PaddingLeft)
				LengthProp(PaddingTop)
				LengthProp(PaddingRight)
				LengthProp(PaddingBottom)
				EnumProp(Position)
				EnumProp(AlignItems)
				EnumPropEx(SelfAlign, AlignItemsType)
				case StyleProperties::AspectRatio:
					ComputedStyle.AspectRatio = value.value.Number;
					break;
				EnumProp(FlexDirection)
				EnumProp(FlexWrap)
				case StyleProperties::FlexBasis:
					ComputedStyle.FlexBasis = value.value.Number;
					break;
				case StyleProperties::FlexGrow:
					ComputedStyle.FlexGrow = value.value.Number;
					break;		
				case StyleProperties::FlexShrink:
						ComputedStyle.FlexShrink = value.value.Number;
						break;
				EnumProp(JustifyContent)
				EnumProp(AlignContent)
				EnumProp(LayoutDirection)
				case StyleProperties::BorderSize:
					ComputedStyle.BorderSize = value.value.Number;
					break;
				case StyleProperties::BorderRadius:
					ComputedStyle.BorderRadius = value.value.Number;
					break;
				case StyleProperties::BorderColor:
					ComputedStyle.BorderColor = value.value.Color / 255.f;
					break;
				case StyleProperties::BackgroundColor:
					this->ComputedStyle.BackgroundColor = value.value.Color / 255.f;
					break;
				case StyleProperties::Color:
					ComputedStyle.FontColor = value.value.Color / 255.f;
					break;
				case StyleProperties::TextAlign:
					ComputedStyle.TextAlign = (TextAlignType)(value.value.Enum);
					break;
				case StyleProperties::Overflow:
					ComputedStyle.Overflow = (bool)value.value.Enum;
					break;
			}
		}

		SetLength(Width)
		SetLength(Height)
		SetLengthNoAuto(MinWidth)
		SetLengthNoAuto(MinHeight)
		SetLengthNoAuto(MaxWidth)
		SetLengthNoAuto(MaxHeight)
		SetLengthBorder(Margin, Left);
		SetLengthBorder(Margin, Top);
		SetLengthBorder(Margin, Right);
		SetLengthBorder(Margin, Bottom);
		SetLengthBorderNoAuto(Padding, Left);
		SetLengthBorderNoAuto(Padding, Top);
		SetLengthBorderNoAuto(Padding, Right);
		SetLengthBorderNoAuto(Padding, Bottom);

		if (ComputedStyle.Position == PositionType::Relative)
			YGNodeStyleSetPositionType(mNode, YGPositionTypeRelative);
		else if (ComputedStyle.Position == PositionType::Absolute)
			YGNodeStyleSetPositionType(mNode, YGPositionTypeAbsolute);

		if (ComputedStyle.FlexDirection == FlexDirectionType::Column)
			YGNodeStyleSetFlexDirection(mNode, YGFlexDirectionColumn);
		else if (ComputedStyle.FlexDirection == FlexDirectionType::ColumnReversed)
			YGNodeStyleSetFlexDirection(mNode, YGFlexDirectionColumnReverse);
		else if (ComputedStyle.FlexDirection == FlexDirectionType::Row)
			YGNodeStyleSetFlexDirection(mNode, YGFlexDirectionRow);
		else if (ComputedStyle.FlexDirection == FlexDirectionType::RowReversed)
			YGNodeStyleSetFlexDirection(mNode, YGFlexDirectionRowReverse);

		if (ComputedStyle.JustifyContent == JustifyContentType::FlexStart)
			YGNodeStyleSetJustifyContent(mNode, YGJustifyFlexStart);
		else if (ComputedStyle.JustifyContent == JustifyContentType::Center)
			YGNodeStyleSetJustifyContent(mNode, YGJustifyCenter);
		else if (ComputedStyle.JustifyContent == JustifyContentType::FlexEnd)
			YGNodeStyleSetJustifyContent(mNode, YGJustifyFlexEnd);
		else if (ComputedStyle.JustifyContent == JustifyContentType::SpaceAround)
			YGNodeStyleSetJustifyContent(mNode, YGJustifySpaceAround);
		else if (ComputedStyle.JustifyContent == JustifyContentType::SpaceBetween)
			YGNodeStyleSetJustifyContent(mNode, YGJustifySpaceBetween);
		else if (ComputedStyle.JustifyContent == JustifyContentType::SpaceEvenly)
			YGNodeStyleSetJustifyContent(mNode, YGJustifySpaceEvenly);

		if (ComputedStyle.AlignItems == AlignItemsType::FlexStart)
			YGNodeStyleSetAlignItems(mNode, YGAlignFlexStart);
		else if (ComputedStyle.AlignItems == AlignItemsType::Center)
			YGNodeStyleSetAlignItems(mNode, YGAlignCenter);
		else if (ComputedStyle.AlignItems == AlignItemsType::FlexEnd)
			YGNodeStyleSetAlignItems(mNode, YGAlignFlexEnd);
		else if (ComputedStyle.AlignItems == AlignItemsType::SpaceAround)
			YGNodeStyleSetAlignItems(mNode, YGAlignSpaceAround);
		else if (ComputedStyle.AlignItems == AlignItemsType::SpaceBetween)
			YGNodeStyleSetAlignItems(mNode, YGAlignSpaceBetween);
		else if (ComputedStyle.AlignItems == AlignItemsType::Stretch)
			YGNodeStyleSetAlignItems(mNode, YGAlignStretch);
		else if (ComputedStyle.AlignItems == AlignItemsType::Baseline)
			YGNodeStyleSetAlignItems(mNode, YGAlignBaseline);
	}

	std::vector<std::shared_ptr<Node>> Node::GetChildrens() const
	{
		return Childrens;
	}
}