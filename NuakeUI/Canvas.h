#pragma once
#include "Node.h"
#include <memory>

namespace NuakeUI
{
	class Canvas
	{
	public:
		Canvas();
		~Canvas() = default;
	private:
		std::shared_ptr<Node> mRootNode;

		void Render();
	};
}