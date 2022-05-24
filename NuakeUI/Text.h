#pragma once
#include "Node.h"
#include "Font.h"

#include <memory>
#include <string>

namespace NuakeUI
{
	enum class TextAlign
	{
		Left, Center, Right
	};

	struct FontStyle
	{
		float FontSize = 64.0f;
		TextAlign Alignment = TextAlign::Left;
		Color FontColor = Color(1, 1, 1, 1);
		Color BackgroundColor = Color(0, 0, 0, 0);
	};

	class Text : public Node
	{
	public:
		std::string Value = "";
		std::shared_ptr<Font> mFont;

		FontStyle FontStyle;

		static std::shared_ptr<Text> New(const std::string& id, const std::string& text);
		Text(const std::string& id, const std::string& text);
		~Text() = default;

		void Calculate() override;
		void Tick() override {};
		void UpdateInput(InputManager* manager) override {};
		void Draw(int z) override;

		float CalculateWidth();
	private:
	};
}