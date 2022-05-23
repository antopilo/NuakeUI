#pragma once
#include "Node.h"

#include <memory>
#include <string>

namespace NuakeUI
{
	class Text : public Node
	{
	public:
		std::string Value = "";

		std::shared_ptr<Text> New(const std::string& text, const std::string& id);

		Text(const std::string& text, const std::string& id) : Value(text)
		{
			ID = id;
		}

		~Text() = default;

		void Draw(int z) override;
	private:
	};
}