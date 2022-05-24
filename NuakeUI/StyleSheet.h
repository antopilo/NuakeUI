#pragma once

#include <memory>

namespace NuakeUI
{
	class StyleSheet
	{
	public:
		static std::shared_ptr<StyleSheet> New()
		{
			return std::make_shared<StyleSheet>();
		}

		StyleSheet() = default;
		~StyleSheet() = default;
	};
}