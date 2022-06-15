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
	private:
		YGConfigRef mYogaConfig;

		std::string mFilePath = "";
		
		InputManager* mInputManager;
		StyleSheetPtr mStyleSheet;

		NodePtr mRootNode;
		
		bool mDirty;
	public:
		static CanvasPtr New();
		Canvas();
		~Canvas();

		void Tick();
		void Draw();
		void ComputeLayout(Vector2 size);
		void ComputeStyle(NodePtr node);
		
		NodePtr GetRoot() const;
		void SetRoot(NodePtr root);

		void SetInputManager(InputManager* manager);

		StyleSheetPtr GetStyleSheet() const;
		void SetStyleSheet(StyleSheetPtr stylesheet);

		template<class T>
		bool FindNodeByID(const std::string& id, std::shared_ptr<T>& node)
		{
			if (!this->mRootNode->FindChildByID<T>(id, node))
				return false;

			return true;
		}
	};
}