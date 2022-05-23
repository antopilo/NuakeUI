#pragma once
#include "Node.h"
#include <NuakeRenderer/Math.h>
#include <memory>

namespace NuakeUI
{
	class Canvas
	{
	public:
		Canvas();
		~Canvas();

		void SetRootNode(std::shared_ptr<Node> root);
		std::shared_ptr<Node> GetRootNode() const;

		void Tick();
		void Draw();
		void Calculate(Vector2 size);

		template<class T> 
		std::shared_ptr<T> FindNodeByID(const std::string & id)
		{
			assert(root != nullptr); // No Root has been set.

			auto result = root->FindChildByID<T>(id);
			assert(result != nullptr);

			return result;
		}

	private:
		YGConfigRef mYogaConfig;
		std::shared_ptr<Node> mRootNode;
	};
}