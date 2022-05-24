#pragma once
#include <NuakeRenderer/Shader.h>
#include <NuakeRenderer/VertexBuffer.h>
#include <NuakeRenderer/VertexArray.h>

#include "Node.h"
#include "Font.h"

#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include <memory>
#include <string>

namespace NuakeUI
{
	class Renderer
	{
	public:
		static Renderer& Get() {
			static Renderer instance;
			return instance;
		}

		void SetViewportSize(const Vector2& size);

		void BeginDraw();
		void DrawNode(std::shared_ptr<Node> node, int z);
		void DrawString(const std::string& string, float fontSize, std::shared_ptr<Font> font = mDefaultFont, Vector3 position = Vector3());
		void DrawChar(std::shared_ptr<Font> font, int letter);
		void DrawRect();


		static std::shared_ptr<Font> mDefaultFont;
	private:
		Vector2 mSize;
		Matrix4 mView;

		Renderer();
		~Renderer() = default;

		std::shared_ptr<NuakeRenderer::Shader> mShader;
		std::shared_ptr<NuakeRenderer::Shader> mSDFShader;
		std::shared_ptr<NuakeRenderer::VertexArray> mVertexArray;
		std::shared_ptr < NuakeRenderer::VertexBuffer> mVertexBuffer;
	};
}