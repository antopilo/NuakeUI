#include "Text.h"
namespace NuakeUI
{
	std::shared_ptr<Text> Text::New(const std::string& id, const std::string& text)
	{
		return std::make_shared<Text>(id, text);
	}

	void Text::Draw(int z)
	{
		Matrix4 transform = Matrix4(1.0f);
		float width = YGNodeLayoutGetWidth(mNode);
		float height = YGNodeLayoutGetHeight(mNode);
		float left = YGNodeLayoutGetLeft(mNode); //+ offset.x;
		float top = YGNodeLayoutGetTop(mNode);// +offset.y;
		top += mFont->LineHeight / 2.0f;
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
		if (Alignment == TextAlign::Center)
			position.x += (width / 2.0f) - CalculateWidth() / 2.0f;
		else if (Alignment == TextAlign::Right)
			position.x += width - CalculateWidth();

		Renderer::Get().DrawString(this->Value, FontSize, mFont, position);
	}

	float Text::CalculateWidth()
	{
		if (Value == "") return 0.f;
		float width = 0.f;
		float fs = FontSize / 64.f;
		float advance = 0.0f;
		for (char const& c : Value)
		{
			Char& letter = mFont->GetChar((int)c);
			width += (letter.Advance);
		}
		return width;
	}
}