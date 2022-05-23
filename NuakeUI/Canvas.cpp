#include "Canvas.h"
#include <yoga/Yoga.h>
#include <yoga/YGConfig.h>

#include "Renderer.h"

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

	void Canvas::Calculate(Vector2 size)
	{
		Renderer::Get().SetViewportSize(size);

		float x, y;
		x = InputManager->GetMouseX();
		y = InputManager->GetMouseY();

		auto root = mRootNode->GetYogaNode();
		if (root)
			YGNodeCalculateLayout(root, size.x, size.y, YGDirectionLTR);
	}
}
