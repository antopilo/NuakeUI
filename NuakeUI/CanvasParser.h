#pragma once
#include "Canvas.h"
#include <memory>

namespace NuakeUI
{
	class CanvasParser
	{
	public:
		static CanvasParser& Get()
		{
			static CanvasParser parser;
			return parser;
		}

		CanvasParser() = default;
		~CanvasParser() = default;

		std::shared_ptr<Canvas> Parse(const std::string& file);
	};
}