#include "Text.h"

#include "Renderer.h"
#include <sstream>

namespace NuakeUI
{
	std::shared_ptr<Text> Text::New(const std::string& id, const std::string& text)
	{
		return std::make_shared<Text>(id, text);
	}

	Text::Text(const std::string& id, const std::string& text) : Value(text)
	{
		ID = id;
		mNode = YGNodeNew();
		mType = NodeType::Text;

		// Split into lines
		auto ss = std::stringstream{ text };
		for (std::string line; std::getline(ss, line, '\n');)
			Lines.push_back(line);

		mFont = Renderer::Get().mDefaultFont;

		YGNodeStyleSetMinHeight(mNode, ((mFont->LineHeight) / 32.f) * FontStyle.FontSize * Lines.size());
		YGNodeStyleSetMinWidth(mNode, CalculateWidth());
		YGNodeStyleSetWidthPercent(mNode, 100.f);
	}

	void Text::Draw(int z)
	{
		const float width = YGNodeLayoutGetWidth(mNode);
		const float height = YGNodeLayoutGetHeight(mNode);

		float left = YGNodeLayoutGetLeft(mNode); 
		float top = YGNodeLayoutGetTop(mNode);

		// Centers the text in the line height.
		top += (mFont->LineHeight / 64.0) * (FontStyle.FontSize) / 2.0f;

		const float paddingLeft = YGNodeLayoutGetPadding(mNode, YGEdgeLeft);
		const float paddingTop = YGNodeLayoutGetPadding(mNode, YGEdgeTop);
		left += paddingLeft;
		top += paddingTop;

		float parentLeft = 0.0f;
		float parentTop = 0.0f;
		auto parent = YGNodeGetParent(mNode);
		while (parent)
		{
			parentLeft += YGNodeLayoutGetLeft(parent);
			parentTop += YGNodeLayoutGetTop(parent);
			parent = YGNodeGetParent(parent);
		}

		const float leftPosition = left + parentLeft;
		const float topPosition = top + parentTop;
		Vector3 position = Vector3(leftPosition, topPosition, z);

		if (FontStyle.Alignment == TextAlign::Center)
		{
			// We center the text horizontally.
			position.x += (width / 2.0f) - CalculateWidth() / 2.0f;
		}
		else if (FontStyle.Alignment == TextAlign::Right)
		{
			// Aligns the line of the left
			position.x += width - CalculateWidth(); 
		}

		// Draw each line and offset the Y of the position by the line height.
		for(int i = 0; i < Lines.size(); i++)
		{
			// Draw the first line
			Renderer::Get().DrawString(Lines[i], FontStyle, mFont, position);
			// Update the Y position to the next line.
			position.y += (mFont->LineHeight / 32.0f) * (FontStyle.FontSize);
		}
	}

	float Text::CalculateWidth()
	{
		// If theres no text, then assume it's 0;
		if (Value == "") return 0.f;

		const float fontSize = FontStyle.FontSize / 64.f;
		float textWidth = 0.f;

		// Iterate over each character and add up the advance.
		for (char const& c : Value)
		{
			Char& letter = mFont->GetChar((int)c);
			textWidth += (letter.Advance);
		}

		// Scale the width by the font size.
		return textWidth * fontSize;
	}

	void Text::Calculate()
	{
		const float halfLineHeight = mFont->LineHeight / 32.f;
		const float linesHeight = Lines.size() * FontStyle.FontSize;
		YGNodeStyleSetMinHeight(mNode, halfLineHeight * linesHeight);
		YGNodeStyleSetMinWidth(mNode, CalculateWidth());
	}
}