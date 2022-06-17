#pragma once
#include "Canvas.h"

#include <memory>
#include <map>
#include <string>
#include <functional>

#include <msdfgen/include/tinyxml2.h>

namespace NuakeUI
{
	typedef std::function<NodePtr(std::string, std::string)> refNew;

	class CanvasParser
	{
	private:
		std::map<std::string, refNew> NodeTypes;
	public:
		CanvasParser();
		~CanvasParser() = default;

		void RegisterNodeType(const std::string& name, refNew refConstructor);
		bool HasNodeType(const std::string& name) const;
		refNew GetNodeType(const std::string& name) const;

		void WriteValueFromString(std::variant<int, float, bool, std::string, char>& var, const std::string& str);

		CanvasPtr Parse(const std::string& file);
		void IterateOverElement(tinyxml2::XMLElement* e, NodePtr node);
		NodePtr CreateNodeFromXML(tinyxml2::XMLElement* xml, const std::string& id = "Node");
		void AddClassesToNode(tinyxml2::XMLElement* e, NodePtr node);
		void AddModelIfToNode(tinyxml2::XMLElement* e, NodePtr node);
		void AddModelClasses(tinyxml2::XMLElement* e, NodePtr node);
	};
}