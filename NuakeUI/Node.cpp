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
	}

	void Node::Tick()
	{

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
	}

	void Node::InsertChild(std::shared_ptr<Node> child)
	{
		Childrens.push_back(child);
		YGNodeInsertChild(this->mNode, child->GetYogaNode(), Childrens.size() - 1);
	}



	std::vector<std::shared_ptr<Node>> Node::GetChildrens() const
	{
		return Childrens;
	}
}