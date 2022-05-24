#pragma once
#include "Node.h"
#include "Font.h"

#include "Renderer.h"
#include <memory>
#include <string>

namespace NuakeUI
{
	enum class TextAlign
	{
		Left, Center, Right
	};

	class Text : public Node
	{
	public:
		std::string Value = "";

		std::shared_ptr<Font> mFont;
		float FontSize = 64.0f;
		TextAlign Alignment = TextAlign::Left;

		static std::shared_ptr<Text> New(const std::string& id, const std::string& text);
		Text(const std::string& id, const std::string& text) : Value(text)
		{
			ID = id;
			mNode = YGNodeNew();
			mFont = Renderer::Get().mDefaultFont;
			//YGNodeStyleSetPadding(mNode, YGEdgeAll, 4.f);
			YGNodeStyleSetMinHeight(mNode, (mFont->LineHeight / 32.f) * FontSize);
			YGNodeStyleSetMinWidth(mNode, CalculateWidth());
			YGNodeStyleSetWidthPercent(mNode, 100.f);
		}

		~Text() = default;

		void Tick() override {};
		void UpdateInput(InputManager* manager) override {};
		void Draw(int z) override;

		float CalculateWidth();
	private:
	};
}