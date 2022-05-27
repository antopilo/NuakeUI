#include "Canvas.h"
#include <yoga/Yoga.h>
#include <yoga/YGConfig.h>

#include "Renderer.h"

#include "Node.h"

namespace NuakeUI
{
	Canvas::Canvas()
	{
		mYogaConfig = YGConfigNew();
		mYogaConfig->useWebDefaults = true;
	}

	Canvas::~Canvas()
	{
		YGConfigFree(mYogaConfig);
		if (mRootNode)
			YGNodeFreeRecursive(mRootNode->GetYogaNode());
	}

	void Canvas::SetRootNode(std::shared_ptr<Node> root)
	{
		mRootNode = root;
	}

	std::shared_ptr<Node> Canvas::GetRootNode() const
	{
		return mRootNode;
	}

	void Canvas::Tick()
	{
		assert(mRootNode); // The canvas doesn't have any root set.

		mRootNode->UpdateInput(InputManager);
		mRootNode->Tick();

		InputManager->ScrollX = 0.f;
		InputManager->ScrollY = 0.f;
	}

	void Canvas::Draw()
	{
		if (mRootNode)
		{
			Renderer::Get().BeginDraw();
			Renderer::Get().DrawNode(mRootNode, 0);
			mRootNode->Draw(0);
		}
	}

	void Canvas::StyleNode(std::shared_ptr<Node> node)
	{
		for (auto& rule : mStyleSheet->Rules)
		{
			bool respectSelector = true;

			for (StyleSelector& selector : rule.Selector)
			{
				bool foundSelector = false;
				if (selector.Type == StyleSelectorType::Class)
				{
					for (auto& c : node->Classes)
					{
						if (c == selector.Value)
							foundSelector = true;
					}
				}
				else if (selector.Type == StyleSelectorType::Pseudo)
				{
					if (selector.Value == "hover" && node->State == NodeState::Hover)
						foundSelector = true;
					if (selector.Value == "active" && node->State == NodeState::Pressed)
						foundSelector = true;
				}
				else if (selector.Type == StyleSelectorType::Id)
				{
					if (node->GetID() == selector.Value)
						foundSelector = true;
				}
				else if (selector.Type == StyleSelectorType::Tag)
				{
					if (selector.Value == "text" &&
						node->GetType() == NodeType::Text)
					{
						foundSelector = true;
					}
					else if (selector.Value == "div" &&
						node->GetType() == NodeType::Node)
					{
						foundSelector = true;
					}
					else if (selector.Value == "button" &&
						node->GetType() == NodeType::Button)
					{
						foundSelector = true;
					}
					else if (selector.Value == "checkbox" &&
						node->GetType() == NodeType::Checkbox)
					{
						foundSelector = true;
					}
				}

				if (!foundSelector)
					respectSelector = false;
			}

			if (respectSelector)
				node->ApplyStyleProperties(rule.Properties);
		}

		for (auto& c : node->GetChildrens())
		{
			StyleNode(c);
		}
	}

	void Canvas::Calculate(Vector2 size)
	{
		Renderer::Get().SetViewportSize(size);

		float x, y;
		x = InputManager->GetMouseX();
		y = InputManager->GetMouseY();

		

		auto root = mRootNode->GetYogaNode();


		// ReStyle the node tree.
		if (Dirty)
			StyleNode(mRootNode);


		mRootNode->Calculate();
		
		if (root)
			YGNodeCalculateLayout(root, size.x, size.y, YGDirectionLTR);
	}
}
