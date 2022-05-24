#include "Renderer.h"
#include "NuakeRenderer/Math.h"
#include <NuakeRenderer/NuakeRenderer.h>
#include <NuakeRenderer/VertexBufferLayout.h>

#include "Font.h"
#include "FontManager.h"

#include "FileSystem.h"

namespace NuakeUI
{
	struct Vertex {
		Vector3 Position;
		Vector2 UV;
	};

	std::shared_ptr<Font> Renderer::mDefaultFont;

	Renderer::Renderer()
	{
		// Rectangle Shader
		std::string vertexSource = FileSystem::ReadFile("../resources/panel.vert.glsl");
		std::string fragSource = FileSystem::ReadFile("../resources/panel.frag.glsl");
		mShader = std::make_shared<NuakeRenderer::Shader>(vertexSource, fragSource);

		// SDF Shader
		vertexSource = FileSystem::ReadFile("../resources/text.vert.glsl");
		fragSource = FileSystem::ReadFile("../resources/text.frag.glsl");
		mSDFShader = std::make_shared<NuakeRenderer::Shader>(vertexSource, fragSource);

		mDefaultFont = FontManager::Get().GetFont("../resources/fonts/SourceSansPro-Regular.ttf");

		const std::vector<Vertex> vertices = {
			{ {  1.f,  1.f, 0.f }, {1.f, 0.f} },
			{ {  1.f, 0.f, 0.f }, {1.f, 1.f} },
			{ { 0.f,  1.f, 0.f }, {0.f, 0.f} },
			{ {  1.f, 0.f, 0.f }, {1.f, 1.f} },
			{ { 0.f, 0.f, 0.f }, {0.f, 1.f} },
			{ { 0.f,  1.f, 0.f }, {0.f, 0.f} },
		};

		mVertexArray = std::make_shared<NuakeRenderer::VertexArray>();
		mVertexArray->Bind();
		mVertexBuffer = std::make_shared<NuakeRenderer::VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
		
		auto vbl = NuakeRenderer::VertexBufferLayout();
		vbl.Push<float>(3); // Position
		vbl.Push<float>(2); // UV

		mVertexArray->AddBuffer(*mVertexBuffer, vbl);
		mVertexArray->Unbind();
	}

	void Renderer::SetViewportSize(const Vector2& size)
	{
		mSize = size;
		mView = glm::ortho(0.f, size.x, size.y, 0.f, -1000.f, 10000.f);
	}

	void Renderer::BeginDraw()
	{
		glViewport(0, 0, mSize.x, mSize.y);
	}

	void Renderer::DrawNode(std::shared_ptr<Node> node, int z)
	{
		Matrix4 transform = Matrix4(1.f);

		const YGNodeRef yogaNode = node->GetYogaNode();
		const float width = YGNodeLayoutGetWidth(yogaNode);
		const float height = YGNodeLayoutGetHeight(yogaNode);
		const float padding = YGNodeLayoutGetPadding(yogaNode, YGEdgeLeft);
		const float left = YGNodeLayoutGetLeft(yogaNode);
		const float top = YGNodeLayoutGetTop(yogaNode);
		const float borderLeft = YGNodeLayoutGetBorder(yogaNode, YGEdgeLeft);

		float parentLeft = 0.f;
		float parentTop = 0.f;
		float parentHeight = 0.f;
		float parentWidth = 0.f;

		auto parent = YGNodeGetParent(yogaNode);
		if (parent)
		{
			parentHeight = YGNodeLayoutGetHeight(parent);
			parentWidth = YGNodeLayoutGetWidth(parent);
		}

		while (parent)
		{
			parentLeft += YGNodeLayoutGetLeft(parent);
			parentTop += YGNodeLayoutGetTop(parent);
			parent = YGNodeGetParent(parent);
		}

		transform = glm::translate(transform, Vector3(left + parentLeft, top + parentTop, z));
		transform = glm::scale(transform, Vector3(width, height, 1.0f));

		mShader->Bind();
		
		mShader->SetUniform("u_Model", transform);
		mShader->SetUniform("u_Size", {width, height});
		mShader->SetUniform("u_View", mView);
		mShader->SetUniform("u_Color", node->Style.background_color);
		mShader->SetUniform("u_Border", node->Style.border);
		mShader->SetUniform("u_BorderColor", node->Style.border_color);

		mVertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		mVertexArray->Unbind();

		mShader->Unbind();
	}

	void Renderer::DrawString(const std::string& string, FontStyle style, std::shared_ptr<Font> font, Vector3 position)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mSDFShader->Bind();
		font->mAtlas->Bind(5);

		const float fontSize = style.FontSize / 64.f;
		float advance = 0.0f;
		for (char const& c : string)
		{
			Char& letter = font->GetChar((int)c);

			// Move the cursor
			Matrix4 model = Matrix4(1.f);
			model = glm::translate(model, position);
			model = glm::translate(model, Vector3(advance * fontSize, (-(letter.PlaneBounds.top) + (font->LineHeight)) * fontSize, 0.f));

			advance += (letter.Advance);

			// Scaling of the quad
			float scaleX = letter.PlaneBounds.right - letter.PlaneBounds.left;
			float scaleY = letter.PlaneBounds.top - letter.PlaneBounds.bottom;
			model = glm::scale(model, Vector3(scaleX * fontSize, scaleY * fontSize, 0.f));
			model = glm::translate(model, Vector3(0, 0, 100));

			// Set Uniforms
			mSDFShader->SetUniforms({
				{ "u_View", mView },
				{ "u_Model", model },
				{ "u_Atlas", (int)5 },
				{ "u_TexturePos",   Vector2(letter.AtlasBounds.Pos.x, letter.AtlasBounds.Pos.y) },
				{ "u_TextureScale", Vector2(letter.AtlasBounds.Size.x, letter.AtlasBounds.Size.y) },
				{ "u_BGColor",   style.BackgroundColor },
				{ "u_FontColor", style.FontColor },
				{ "u_PxRange",   fontSize },
			});

			// Draw glyps
			mVertexArray->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	void Renderer::DrawChar(std::shared_ptr<Font> font, int letter)
	{
		
	}
}