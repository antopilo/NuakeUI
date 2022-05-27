#include "Text.h"

#include "Renderer.h"
#include <sstream>

namespace NuakeUI
{
	std::shared_ptr<Text> Text::New(const std::string& id, const std::string& text)
	{
		return std::make_shared<Text>(id, text);
	}

	Text::Text(const std::string& id, const std::string& text)
	{
		ID = id;
		mNode = YGNodeNew();
		mType = NodeType::Text;

		mFont = Renderer::Get().mDefaultFont;

		SetText(text);
		float height = ((mFont->LineHeight) / 32.f) * ComputedStyle.FontSize * Lines.size();
		YGNodeStyleSetHeight(mNode, height);
		YGNodeStyleSetMinWidth(mNode, CalculateWidth());
		YGNodeStyleSetWidthPercent(mNode, 100.f);
	}

	void Text::SetText(const std::string& text)
	{
		Lines.clear();

		// Split into lines
		auto ss = std::stringstream{ text };
		for (std::string line; std::getline(ss, line, '\n');)
			Lines.push_back(line);
	}

	void Text::Draw(int z)
	{
		const float width = YGNodeLayoutGetWidth(mNode);
		const float height = YGNodeLayoutGetHeight(mNode);
		ComputedSize = { width, height };

		float left = YGNodeLayoutGetLeft(mNode); 
		float top = YGNodeLayoutGetTop(mNode);

		// Centers the text in the line height.
		top += (mFont->LineHeight / 64.0) * (ComputedStyle.FontSize) / 2.0f;

		const float paddingLeft = YGNodeLayoutGetPadding(mNode, YGEdgeLeft);
		const float paddingTop = YGNodeLayoutGetPadding(mNode, YGEdgeTop);
		left += paddingLeft;
		top += paddingTop;

		float parentLeft = 0.0f;
		float parentTop = 0.0f;
		auto parent = Parent;
		//while (parent)
		//{
		parentLeft += parent->ComputedPosition.x;
		parentTop += parent->ComputedPosition.y;
			//parent = parent->Parent;
		//}

		const float leftPosition = left + parentLeft;
		const float topPosition = top + parentTop;
		Vector3 position = Vector3(leftPosition, topPosition, z);
		ComputedPosition = position;

		if (ComputedStyle.TextAlign == TextAlignType::Center)
		{
			// We center the text horizontally.
			position.x += (width / 2.0f) - CalculateWidth() / 2.0f;
		}
		else if (ComputedStyle.TextAlign == TextAlignType::Right)
		{
			// Aligns the line of the left
			position.x += width - CalculateWidth(); 
		}

		// Scissor the parent bounding box.
		bool hideOverflow = Parent != nullptr && Parent->ComputedStyle.Overflow == OverflowType::Hidden;
		if (hideOverflow)
		{
			glEnable(GL_SCISSOR_TEST);
			glScissor(Parent->ComputedPosition.x, (1080 - Parent->ComputedPosition.y - Parent->ComputedSize.y), Parent->ComputedSize.x, Parent->ComputedSize.y);
		}

		// Draw each line and offset the Y of the position by the line height.
		for(int i = 0; i < Lines.size(); i++)
		{
			// Draw the first line
			Renderer::Get().DrawString(Lines[i], ComputedStyle, mFont, position);
			// Update the Y position to the next line.
			position.y += (mFont->LineHeight / 32.0f) * (ComputedStyle.FontSize);
		}

		// Disable scissoring.
		if (hideOverflow)
			glDisable(GL_SCISSOR_TEST);
	}

	float Text::CalculateWidth()
	{
		// If theres no text, then assume it's 0;
		if (Lines.size() == 0) return 0.f;

		const float fontSize = ComputedStyle.FontSize / 64.f;
		
		// Find the largest line.
		float maxWidth = 0.f;
		for (auto& l : Lines)
		{
			float textWidth = 0.f;
			// Iterate over each character and add up the advance.
			for (char const& c : l)
			{
				Char& letter = mFont->GetChar((int)c);
				textWidth += (letter.Advance);
			}

			if (textWidth > maxWidth)
				maxWidth = textWidth;
		}

		// Scale the width by the font size.
		return maxWidth * fontSize;
	}

	void Text::Calculate()
	{
		const float halfLineHeight = mFont->LineHeight / 32.f;
		const float linesHeight = Lines.size() * ComputedStyle.FontSize;
		YGNodeStyleSetHeight(mNode, halfLineHeight * linesHeight);
		YGNodeStyleSetWidth(mNode, CalculateWidth());
	}
}