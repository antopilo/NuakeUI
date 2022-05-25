#include "StyleSheetParser.h"

#include <cassert>
#include <vector>

#include "FileSystem.h"

namespace NuakeUI
{
	std::shared_ptr<StyleSheet> StyleSheetParser::Parse(const std::string& path)
	{
		assert(FileSystem::FileExists(path));
		std::string fileContent = FileSystem::ReadFile(path);

		auto styleSheet = StyleSheet::New();

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
				ParseStyleRule(rule, styleSheet);
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
		
		return styleSheet;
	}

	void StyleSheetParser::ParseImportRule(KatanaRule* rule)
	{
		auto importRule = reinterpret_cast<KatanaImportRule*>(rule);
		std::string path = importRule->href;
	}

	StyleProperties GetPropFromString(const std::string& prop)
	{
		if (prop == "height") return StyleProperties::Height;
		else if (prop == "max-height")		return StyleProperties::MaxHeight;
		else if (prop == "min-height")		return StyleProperties::MinHeight;
		else if (prop == "width")			return StyleProperties::Width;
		else if (prop == "max-width")		return StyleProperties::MaxWidth;
		else if (prop == "min-width")		return StyleProperties::MinWidth;
		else if (prop == "padding-left")	return StyleProperties::PaddingLeft;
		else if (prop == "padding-right")	return StyleProperties::PaddingRight;
		else if (prop == "padding-top")		return StyleProperties::PaddingTop;
		else if (prop == "padding-bottom")	return StyleProperties::PaddingBottom;
		else if (prop == "margin-left")		return StyleProperties::MarginLeft;
		else if (prop == "margin-right")	return StyleProperties::MarginRight;
		else if (prop == "margin-top")		return StyleProperties::MarginTop;
		else if (prop == "margin-bottom")	return StyleProperties::MarginBottom;
		else if (prop == "background-color")	return StyleProperties::BackgroundColor;
	}

	void StyleSheetParser::ParseStyleRule(KatanaRule* rule, std::shared_ptr<StyleSheet> styleSheet)
	{
		auto styleRule = reinterpret_cast<KatanaStyleRule*>(rule);
		std::string styleName = rule->name;
		auto styleSelector = std::vector<StyleSelector>();
		for (int s = 0; s < styleRule->selectors->length; s++)
		{
			void** selectorData = styleRule->selectors->data;
			void** declarationData = styleRule->declarations->data;

			auto selector = reinterpret_cast<KatanaSelector*>(selectorData[s]);
			auto match = selector->match; // tag, id or class
			switch (match)
			{
			case KatanaSelectorMatchTag:
				{
					std::string matchTag = selector->data->value;
					styleSelector.push_back({ StyleSelectorType::Tag, matchTag });
				}
				break;

			case KatanaSelectorMatchId:
				{
					std::string matchId = selector->data->value;
					styleSelector.push_back({ StyleSelectorType::Id, matchId });
				}
				break;

			case KatanaSelectorMatchClass:
				{
					std::string matchClass = selector->data->value;
					styleSelector.push_back({ StyleSelectorType::Class, matchClass });
				}
				break;
			}
		}

		// Added the new rule with selectors.
		auto newRule = StyleRule(styleSelector);
		
		// Now add the properties to the new rule.
		for (int d = 0; d < styleRule->declarations->length; d++)
		{
			void** declarationData = styleRule->declarations->data;
			auto declaration = reinterpret_cast<KatanaDeclaration*>(declarationData[d]);
			std::string prop = declaration->property;

			StyleProperties propType = GetPropFromString(prop);
			PropValue propValue;
			auto values = declaration->values;
			for (auto v = 0; v < values->length; v++)
			{
				KatanaValue* value = (KatanaValue*)values->data[v];
				switch (value->unit)
				{
				case KatanaValueUnit::KATANA_VALUE_PX:
					{	
						// do pixel
						propValue.value.Number = value->fValue;
						propValue.type = PropValueType::Pixel;
					}
					break;
				case KatanaValueUnit::KATANA_VALUE_PERCENTAGE:
					{
						// do percentage
						propValue.value.Number = value->fValue;
						propValue.type = PropValueType::Percent;
					}
					break;
				case KatanaValueUnit::KATANA_VALUE_PARSER_HEXCOLOR:
				{
					int r, g, b, a = 255;
					sscanf(value->string, "%02x%02x%02x%02x", &r, &g, &b, &a);
					propValue.value.Color = Color(r, g, b, a);
					propValue.type = PropValueType::Color;
				}
				break;
				case KatanaValueUnit::KATANA_VALUE_RGBCOLOR:
				{
					if (value->unit == KATANA_VALUE_RGBCOLOR)
					{
						printf(value->raw);
					}
					
					//propValue.value.Color = value->raw;
					propValue.type = PropValueType::Color;
				}
				break;
				case KatanaValueUnit::KATANA_VALUE_UNKNOWN:
				{
					// other
					std::string valueStr = value->string;
				}
				break;
				}
			}

			newRule.SetProp(propType, propValue);
		}

		styleSheet->Rules.push_back(newRule);
	}
}