#pragma once
#include <NuakeRenderer/Shader.h>
#include <NuakeRenderer/VertexBuffer.h>
#include <NuakeRenderer/VertexArray.h>
#include "Node.h"

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
		void DrawRect();
	private:
		Vector2 mSize;
		Matrix4 mView;

		Renderer();
		~Renderer() = default;

		std::shared_ptr<NuakeRenderer::Shader> mShader;
		std::shared_ptr<NuakeRenderer::VertexArray> mVertexArray;
		std::shared_ptr < NuakeRenderer::VertexBuffer> mVertexBuffer;
	};
}