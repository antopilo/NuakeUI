#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

namespace NuakeUI
{
	namespace StringHelper
	{
		/// <summary>
		/// Splits a string into a vector of substrings.
		/// </summary>
		/// <param name="string">The string to be split</param>
		/// <param name="delimiter">The token</param>
		/// <returns></returns>
		std::vector<std::string> Split(std::string string, const char delimiter)
		{
			auto splittedString = std::vector<std::string>();

			std::stringstream ss(string);
			std::string element;

			while (getline(ss, element, delimiter))
			{
				splittedString.push_back(element);
			}

			return splittedString;
		}
	}
}