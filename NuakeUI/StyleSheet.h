#pragma once


#include <memory>

namespace NuakeUI
{
	class StyleSheet
	{

	public:
		static std::shared_ptr<StyleSheet> New()
		{
			std::make_shared<StyleSheet>();
		}

		StyleSheet();
		~StyleSheet() = default;



	};
}