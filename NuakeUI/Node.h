#pragma once
#include "InputManager.h"

#include <NuakeRenderer/Math.h>
#include <yoga/yoga.h>

#include "NodeState.h"
#include "DataBindObject.h"

#include <map>
#include <vector>
#include <string>
#include "StyleSheet.h"
#include <any>
#include <NuakeUI/DataModelOperations.h>
#include <NuakeUI/NodeStyle.h>

#define SetLength(name) \
if (ComputedStyle.##name.type == LengthType::Auto)  \
YGNodeStyleSet##name##Auto(mNode); \
else if (ComputedStyle.##name.type == LengthType::Pixel) YGNodeStyleSet##name(mNode, ComputedStyle.##name.value); \
else if (ComputedStyle.##name.type == LengthType::Percentage) YGNodeStyleSet##name##Percent(mNode, ComputedStyle.##name.value); \

#define SetLengthBorder(name, border) \
if (ComputedStyle.##name##border.type == LengthType::Auto)  \
YGNodeStyleSet##name##Auto(mNode, YGEdge##border); \
else if (ComputedStyle.##name##border.type == LengthType::Pixel) YGNodeStyleSet##name(mNode, YGEdge##border, ComputedStyle.##name##border.value); \
else if (ComputedStyle.##name##border.type == LengthType::Percentage) YGNodeStyleSet##name##Percent(mNode,  YGEdge##border, ComputedStyle.##name##border.value); \

#define SetLengthBorderNoAuto(name, border) \
if (ComputedStyle.##name##border.type == LengthType::Pixel) YGNodeStyleSet##name(mNode, YGEdge##border, ComputedStyle.##name##border.value); \
else if (ComputedStyle.##name##border.type == LengthType::Percentage) YGNodeStyleSet##name##Percent(mNode,  YGEdge##border, ComputedStyle.##name##border.value); \


#define SetLengthNoAuto(name) \
if (ComputedStyle.##name.type == LengthType::Pixel) YGNodeStyleSet##name(mNode, ComputedStyle.##name.value); \
else if (ComputedStyle.##name.type == LengthType::Percentage) YGNodeStyleSet##name##Percent(mNode, ComputedStyle.##name.value); \

#define EnumProp(name) EnumPropEx(name, ##name##Type)

#define EnumPropEx(name, enums) \
case StyleProperties::name: \
{ \
auto type = value.value.Enum; \
ComputedStyle.##name## = (##enums##)type; \
} \
break;\

#define LengthProp(name)  \
				case StyleProperties::name: \
				{ \
					switch (value.type) \
					{ \
					case PropValueType::Pixel: \
					{ \
						ComputedStyle.name.type = LengthType::Pixel; \
						ComputedStyle.name.value = value.value.Number; \
					} \
					break; \
					case PropValueType::Percent: \
					{ \
						ComputedStyle.name.type = LengthType::Percentage;  \
						ComputedStyle.name.value = value.value.Number; \
					} \
					break; \
					case PropValueType::Auto: \
					{ \
						ComputedStyle.name.type = LengthType::Auto; \
					} \
					break; \
					} \
				} \
break; \


namespace NuakeUI
{
	

	enum class NodeType
	{
		Node, Button, Checkbox, Text
	};

	class Node;
	typedef std::shared_ptr<Node> NodePtr;
	
	class Node
	{
	protected:
		std::string ID = ""; // Name of the node in the tree
		YGNodeRef mNode; // Yoga_CPP nodes.
		std::vector<NodePtr> Childrens = std::vector<NodePtr>();
		NodeType mType = NodeType::Node;

		DataModelPtr mDataModel;
	public:
		float ScrollDelta = 0.0f;
		float MaxScrollDelta = 0.0f;

		Vector2 ComputedSize = {0, 0};
		Vector2 ComputedPosition = {0, 0};
		Node* Parent = nullptr;

		NodeState State = NodeState::Idle;
		NodeStyle ComputedStyle; // The current visual styles.
		
		std::shared_ptr<DataModelOperation> ModelIf;

		std::vector<std::string> Classes = std::vector<std::string>(); // List of css classes.
		void AddClass(const std::string& c) { Classes.push_back(c); }
		void RemoveClass(const std::string& c)
		{
			bool found = false;
			int i = 0;
			for (auto& cc : Classes)
			{
				if (cc == c)
				{
					found = true;
					break;
				}
				i++;
			}

			if(found)
				Classes.erase(Classes.begin() + i);
		}
		bool HasClass(const std::string& c) const
		{
			bool found = false;
			for (auto& cc : Classes)
				if (cc == c) found = true;
			return found;
		}

		// Should be used to creates nodes since it creates a shared_ptr for you.
		static NodePtr New(const std::string id);

		Node(const std::string& id); // Do not use.
		Node() = default;
		~Node() = default;

		inline NodeType GetType() const { return mType; }
	public:
		std::any UserData;

		virtual void Draw(int z);
		virtual void UpdateInput(InputManager* manager);
		virtual void Tick();

		void ApplyStyleProperties(std::map<StyleProperties, PropValue> properties);

		virtual void Calculate();

		bool IsMouseHover(float x, float y);

		YGNodeRef GetYogaNode() const { return mNode; }
		std::string GetID() const { return ID; }

		bool HasDataModel() const { return mDataModel != nullptr; }
		DataModelPtr GetDataModel() const { return mDataModel; }
		void SetDataModel(DataModelPtr dataModel)
		{
			mDataModel = dataModel;
		}

		// Childrens
		std::vector<NodePtr> GetChildrens() const;
		void InsertChild(NodePtr child);



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

			assert(false);  // Node not found.
		}

		uint32_t GetIndex() const
		{
			if (!Parent)
				return -1;

			int i = 0;
			for (auto& c : Parent->GetChildrens())
			{
				if (c->mNode == mNode)
					return i;

				i++;
			}

			return 0;
		}

		template<class T>
		bool FindChildByID(const std::string& id, std::shared_ptr<T>& node)
		{
			for (auto& c : Childrens)
			{
				if (c->ID == id)
				{
					node = std::static_pointer_cast<T>(c);
					return true;
				}

				if (c->FindChildByID<T>(id, node))
					return true;
			}

			return false;
		}

		std::shared_ptr<DataModel> GetModel()
		{
			// Travel updward to find model
			if (HasDataModel())
				return GetDataModel();

			else if (Parent != nullptr)
				return Parent->GetModel();
			else
				return nullptr;
		}
	};
}