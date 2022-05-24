#include "StyleSheetParser.h"

#include <cassert>

#include "FileSystem.h"

namespace NuakeUI
{
	std::shared_ptr<StyleSheet> StyleSheetParser::Parse(const std::string& path)
	{
		assert(FileSystem::FileExists(path));
		std::string fileContent = FileSystem::ReadFile(path);

		auto data = katana_parse(fileContent.c_str(), fileContent.length(), KatanaParserModeStylesheet);
		for (int i = 0; i < data->stylesheet->rules.length; i++)
		{
			KatanaRule* rule = (KatanaRule*)data->stylesheet->rules.data[i];

			auto ruleType = rule->type;
			switch (ruleType)
			{
			case KatanaRuleImport:
				ParseImportRule(rule);
				break;
			case KatanaRuleStyle:
				ParseStyleRule(rule);
				break;
			}
		}

		// Print out errors.
		if (data->errors.length > 0)
		{
			KatanaArray errors = data->errors;
			for (int i = 0; i < errors.length; i++)
			{
				KatanaError* error = (KatanaError*)errors.data[i];
				printf(error->message);
				printf("\n");
			}
			return false;
		}

		auto styleSheet = StyleSheet::New();
		return styleSheet;
	}

	void StyleSheetParser::ParseImportRule(KatanaRule* rule)
	{
		auto importRule = reinterpret_cast<KatanaImportRule*>(rule);
		std::string path = importRule->href;

	}

	void StyleSheetParser::ParseStyleRule(KatanaRule* rule)
	{
		auto styleRule = reinterpret_cast<KatanaStyleRule*>(rule);
		for (int s = 0; s < styleRule->selectors->length; s++)
		{
			void** selectorData = styleRule->selectors->data;
			auto selector = reinterpret_cast<KatanaSelector*>(selectorData[s]);
			auto match = selector->match; // tag, id or class
			switch (match)
			{
			case KatanaSelectorMatchTag:
				{
					std::string matchTag = selector->data->value;
				}
				break;

			case KatanaSelectorMatchId:
				{
					std::string matchId = selector->data->value;
				}
				break;

			case KatanaSelectorMatchClass:
				{
					std::string matchClass = selector->data->value;
				}
				break;
			}
		}

		for (int d = 0; d < styleRule->declarations->length; d++)
		{
			void** declarationData = styleRule->declarations->data;
			auto declaration = reinterpret_cast<KatanaDeclaration*>(declarationData[d]);
			std::string prop = declaration->property;
			auto values = declaration->values;
			for (auto v = 0; v < values->length; v++)
			{
				KatanaValue* value = (KatanaValue*)values->data[v];
				switch (value->unit)
				{
				case KatanaValueUnit::KATANA_VALUE_PX:
					{	
						// do pixel
						float valuePx = value->fValue;
					}
					break;
				case KatanaValueUnit::KATANA_VALUE_PERCENTAGE:
					{
						// do percentage
						float valuePerc = value->fValue;

					}
					break;
				case KatanaValueUnit::KATANA_VALUE_UNKNOWN:
					{
						// other
						
					}
					break;
				}
			}
		}
	}
}