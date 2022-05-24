#pragma once
#include "Node.h"
#include "InputManager.h"

#include <NuakeRenderer/Math.h>
#include <memory>

namespace NuakeUI
{
	class Canvas
	{
	public:
		static std::shared_ptr<Canvas> New() {
			return std::make_shared<Canvas>();
		}

		Canvas();
		~Canvas();

		void SetRootNode(std::shared_ptr<Node> root);
		std::shared_ptr<Node> GetRootNode() const;

		void Tick();
		void Draw();
		void Calculate(Vector2 size);

		void SetInputManager(InputManager* manager)
		{
			InputManager = manager;
		}

		template<class T>
		std::shared_ptr<T> FindNodeByID(const std::string& id)
		{
			assert(root != nullptr); // No Root has been set.

			auto result = root->FindChildByID<T>(id);
			assert(result != nullptr);

			return result;
		}

	private:
		std::string mFilePath = "";
		YGConfigRef mYogaConfig;
		std::shared_ptr<Node> mRootNode;
		InputManager* InputManager;
	};
}