#pragma once
#include "Node.h"
#include "InputManager.h"
#include "StyleSheet.h"

#include <NuakeRenderer/Math.h>
#include <memory>

namespace NuakeUI
{
	class Canvas;
	typedef std::shared_ptr<Canvas> CanvasPtr;
	
	class Canvas
	{
	public:
		bool Dirty = false;
		static CanvasPtr New() 
		{
			return std::make_shared<Canvas>();
		}

		Canvas();
		~Canvas();

		void SetRootNode(std::shared_ptr<Node> root);
		std::shared_ptr<Node> GetRootNode() const;

		const std::shared_ptr<StyleSheet> GetStyleSheet() const { return mStyleSheet; }
		void SetStyleSheet(std::shared_ptr<StyleSheet> stylesheet) { Dirty = true; mStyleSheet = stylesheet; }

		void Tick();
		void Draw();

		void StyleNode(std::shared_ptr<Node> node);
		void Calculate(Vector2 size);

		void SetInputManager(InputManager* manager)
		{
			InputManager = manager;
		}

		template<class T>
		bool FindNodeByID(const std::string& id, std::shared_ptr<T>& node)
		{
			if (!this->mRootNode->FindChildByID<T>(id, node))
				return false;

			return true;
		}

	private:
		std::string mFilePath = "";
		YGConfigRef mYogaConfig;
		std::shared_ptr<Node> mRootNode;
		InputManager* InputManager;
		std::shared_ptr<StyleSheet> mStyleSheet;
	};
}