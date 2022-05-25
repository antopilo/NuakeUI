#include "Node.h"
#include "Renderer.h"

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
			mState = State::Idle;
		else
			mState = State::Hover;

		if (isHover && inputManager->IsMouseInputDown())
			mState = State::Clicked;

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
			}
		}

		SetLength(Width)
		SetLength(Height)
		SetLengthNoAuto(MinWidth)
		SetLengthNoAuto(MinHeight)
		SetLengthNoAuto(MaxWidth)
		SetLengthNoAuto(MaxHeight)
	}

	std::vector<std::shared_ptr<Node>> Node::GetChildrens() const
	{
		return Childrens;
	}
}