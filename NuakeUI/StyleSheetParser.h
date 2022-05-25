#pragma once

#include <string>
#include <memory>

#include "Vendors/katana-parser/katana.h"

#include "StyleSheet.h"

namespace NuakeUI
{
	class StyleSheetParser
	{
	public:

		static StyleSheetParser& Get()
		{
			static StyleSheetParser parser;
			return parser;
		}

		StyleSheetParser() = default;
		~StyleSheetParser() = default;

		std::shared_ptr<StyleSheet> Parse(const std::string& path);

	private:
		void ParseImportRule(KatanaRule* rule);
		void ParseStyleRule(KatanaRule* rule, std::shared_ptr<StyleSheet> stylesheet);
	};
}