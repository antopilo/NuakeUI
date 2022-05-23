#pragma once
#include <yoga/yoga.h>
#include <map>
#include <vector>
#include <string>
#include <NuakeRenderer/Math.h>

namespace NuakeUI
{
	struct NodeStyle
	{
		Color background_color;
		Color border_color;
		float border = 0.f;
	};

	class Node
	{
	public:
		NodeStyle Style;

		std::vector<std::string> Classes = std::vector<std::string>();
		std::string ID = "";

		static std::shared_ptr<Node> New(const std::string id);

		Node() = default;
		Node(const std::string& id);
		~Node() = default;

		YGNodeRef GetYogaNode() const { return mNode; }

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

			// Node not found.
			assert(false); 
		}

		template<class T>
		std::shared_ptr<T> FindChildByID(const std::string& id)
		{
			assert(Childrens.size() > 0); // No childrens.
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

		std::vector<std::shared_ptr<Node>> GetChildrens() const;

		virtual void Draw(int z);
		void Tick();
		void Calculate();

		std::string GetID() const { return ID; }

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
		void SetBorder(float w)  { Style.border = w; }
		void SetBorderColor(Color c) { Style.border_color = c; }
		YGNodeRef mNode;
	private:
		std::vector<std::shared_ptr<Node>> Childrens = std::vector<std::shared_ptr<Node>>();
	};
}