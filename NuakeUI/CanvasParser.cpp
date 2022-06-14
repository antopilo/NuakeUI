#include "CanvasParser.h"

#include "Canvas.h"

#include "Text.h"
#include "Button.h"

#include "FileSystem.h"
#include "StyleSheetParser.h"
#include "StringHelper.h"

#include <msdfgen/include/tinyxml2.h>

namespace NuakeUI
{
	void IterateOverElement(tinyxml2::XMLElement* e, std::shared_ptr<Node> node)
	{
		tinyxml2::XMLElement* current = e;
		while (current)
		{
			std::string id = "Node";

			// Look if the node has an id.
			auto idAttribute = current->FindAttribute("id");
			if (idAttribute)
				id = idAttribute->Value();

			std::shared_ptr<Node> newNode;

			// Scan the class attribute
			std::vector<std::string> classes;
			auto classAttribute = current->FindAttribute("class");
			if (classAttribute) // If the attribute is found.
			{
				std::string strClasses = classAttribute->Value();

				// Split the classes with space.
				classes = StringHelper::Split(strClasses, ' ');
			}
			
			// Parse the type of the node.
			std::string type = current->Value();
			if (type == "div") // Regular node.
			{
				// Use default name already placed.
				newNode = Node::New(id);
			}
			else if (type == "text") // Text node.
			{
				// Change the default id to the type of the node.
				if (id == "Node") id = "Text";
				std::string text = current->GetText() ? current->GetText() : "";
				newNode = Text::New(id, text);
			}
			else if (type == "button") // Button node.
			{
				// Change the default id to the type of the node.
				if (id == "Node") id = "Button";
				std::string text = current->GetText() ? current->GetText() : "";
				newNode =  Button::New(id, text);
			}

			// Add the classes to the node.
			newNode->Classes = classes;

			// Insert in the tree
			node->InsertChild(newNode);

			// Recursivity on the childs of the current node.
			IterateOverElement(current->FirstChildElement(), newNode);

			// Continue to the sibbling after going Depth first.
			current = current->NextSiblingElement();
		}
	}

	std::shared_ptr<Canvas> CanvasParser::Parse(const std::string& path)
	{
		tinyxml2::XMLDocument doc;
		if (tinyxml2::XMLError error = doc.LoadFile(path.c_str()))
		{
			std::cout << "Error loading UI file: " << path << "\n" <<
				"Error code is: " << error << std::endl;

			return nullptr;
		}

		auto canvas = Canvas::New();
		auto root = Node::New("root");
		auto parent = doc.FirstChildElement("div");

		// Look for stylesheet attribute in root.
		auto styleSheet = parent->FindAttribute("stylesheet");
		if (styleSheet)
		{
			std::string relativePath = path + "/../" + styleSheet->Value();
			if (FileSystem::FileExists(relativePath));
			{
				auto styleSheet = StyleSheetParser::Get().Parse(relativePath);
				canvas->SetStyleSheet(styleSheet);
			}
		}

		IterateOverElement(parent, root);

		canvas->SetRootNode(root);
		return canvas;
	}
}