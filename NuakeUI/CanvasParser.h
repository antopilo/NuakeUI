#pragma once
#include "Canvas.h"

#include <memory>

#include <msdfgen/include/tinyxml2.h>

namespace NuakeUI
{
	class CanvasParser
	{
	public:
		CanvasParser() = default;
		~CanvasParser() = default;

		void WriteValueFromString(std::variant<int, float, bool, std::string, char>& var, const std::string& str);

		CanvasPtr Parse(const std::string& file);
		void IterateOverElement(tinyxml2::XMLElement* e, NodePtr node);
		NodePtr CreateNodeFromXML(tinyxml2::XMLElement* xml, const std::string& id = "Node");
		void AddClassesToNode(tinyxml2::XMLElement* e, NodePtr node);
		void AddModelIfToNode(tinyxml2::XMLElement* e, NodePtr node);
		void AddModelClasses(tinyxml2::XMLElement* e, NodePtr node);
	};
}