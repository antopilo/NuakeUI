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

		std::string RemoveChar(std::string string, const char character)
		{
			// remove space from string
			string.erase(std::remove(string.begin(), string.end(), ' '), string.end());
			return string;
		}

		std::vector<std::string> Split(std::string s, const std::string& delimiter)
		{
			auto splittedString = std::vector<std::string>();

			std::vector<std::string> splits;
			size_t pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				splits.push_back(token);
				s.erase(0, pos + delimiter.length());
			}
			splits.push_back(s);

			return splits;
		}
	}
}