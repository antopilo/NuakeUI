#pragma once
#include "Canvas.h"

#include <memory>

namespace NuakeUI
{
	class CanvasParser
	{
	public:
		CanvasParser() = default;
		~CanvasParser() = default;

		CanvasPtr Parse(const std::string& file);
	};
}