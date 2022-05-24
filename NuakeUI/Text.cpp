#include "Text.h"

#include "Renderer.h"

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

		mFont = Renderer::Get().mDefaultFont;
		//YGNodeStyleSetPadding(mNode, YGEdgeAll, 4.f);
		YGNodeStyleSetMinHeight(mNode, (mFont->LineHeight / 32.f) * FontStyle.FontSize);
		YGNodeStyleSetMinWidth(mNode, CalculateWidth());
		YGNodeStyleSetWidthPercent(mNode, 100.f);
	}

	void Text::Draw(int z)
	{
		Matrix4 transform = Matrix4(1.0f);
		float width = YGNodeLayoutGetWidth(mNode);
		float height = YGNodeLayoutGetHeight(mNode);
		float left = YGNodeLayoutGetLeft(mNode); //+ offset.x;
		float top = YGNodeLayoutGetTop(mNode);// +offset.y;
		top += (mFont->LineHeight / 64.0) * (FontStyle.FontSize) / 2.0f;
		const float padding = YGNodeLayoutGetPadding(mNode, YGEdgeLeft);
		left += padding;
		top += YGNodeLayoutGetPadding(mNode, YGEdgeTop);

		float parentLeft = 0.0f;
		float parentTop = 0.0f;
		auto parent = YGNodeGetParent(mNode);

		float maxWidth = 0;
		if (parent)
			maxWidth = YGNodeLayoutGetWidth(parent);

		while (parent)
		{
			parentLeft += YGNodeLayoutGetLeft(parent);
			parentTop += YGNodeLayoutGetTop(parent);
			parent = YGNodeGetParent(parent);
		}

		Vector3 position = Vector3(left + parentLeft, top + parentTop, z);
		if (FontStyle.Alignment == TextAlign::Center)
			position.x += (width / 2.0f) - CalculateWidth() / 2.0f;
		else if (FontStyle.Alignment == TextAlign::Right)
			position.x += width - CalculateWidth();

		Renderer::Get().DrawString(this->Value, FontStyle, mFont, position);
	}

	float Text::CalculateWidth()
	{
		if (Value == "") return 0.f;
		float width = 0.f;
		float fs = FontStyle.FontSize / 64.f;
		float advance = 0.0f;
		for (char const& c : Value)
		{
			Char& letter = mFont->GetChar((int)c);
			width += (letter.Advance);
		}
		return width * fs;
	}

	void Text::Calculate()
	{
		YGNodeStyleSetMinWidth(mNode, CalculateWidth());
		YGNodeStyleSetMinHeight(mNode, (mFont->LineHeight / 32.f) * FontStyle.FontSize);
	}
}