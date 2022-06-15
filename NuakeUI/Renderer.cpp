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
		ReloadShaders();

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
		mVertexBuffer = std::make_shared<NuakeRenderer::VertexBuffer>(vertices.data(), (unsigned int)(vertices.size() * sizeof(Vertex)));
		
		auto vbl = NuakeRenderer::VertexBufferLayout();
		vbl.Push<float>(3); // Position
		vbl.Push<float>(2); // UV

		mVertexArray->AddBuffer(*mVertexBuffer, vbl);
		mVertexArray->Unbind();
	}

	void Renderer::ReloadShaders()
	{
		// Rectangle Shader
		std::string vertexSource = FileSystem::ReadFile("../resources/panel.vert.glsl");
		std::string fragSource = FileSystem::ReadFile("../resources/panel.frag.glsl");
		mShader = std::make_shared<NuakeRenderer::Shader>(vertexSource, fragSource);

		// SDF Shader
		vertexSource = FileSystem::ReadFile("../resources/text.vert.glsl");
		fragSource = FileSystem::ReadFile("../resources/text.frag.glsl");
		mSDFShader = std::make_shared<NuakeRenderer::Shader>(vertexSource, fragSource);
	}

	void Renderer::SetViewportSize(const Vector2& size)
	{
		mSize = size;
		mView = glm::ortho(0.f, size.x, size.y, 0.f, -1000.f, 10000.f);
	}

	void Renderer::BeginDraw()
	{
		int viewportW = (int)mSize.x;
		int viewportH = (int)mSize.y;
		glViewport(0, 0, viewportW, viewportH);
	}

	void Renderer::DrawNode(std::shared_ptr<Node> node, int z)
	{
		Matrix4 transform = Matrix4(1.f);

		float parentScroll = 0.f;
		float parentPaddingRight = 0.f;
		if (node->Parent)
		{
			parentScroll = node->Parent->ScrollDelta;
		}

		const YGNodeRef yogaNode = node->GetYogaNode();
		const float width = YGNodeLayoutGetWidth(yogaNode) - parentPaddingRight;
		const float height = YGNodeLayoutGetHeight(yogaNode);
		const float padding = YGNodeLayoutGetPadding(yogaNode, YGEdgeLeft);
		const float margin = YGNodeLayoutGetMargin(yogaNode, YGEdgeLeft);
		const float marginTop = YGNodeLayoutGetMargin(yogaNode, YGEdgeTop);
		const float left = YGNodeLayoutGetLeft(yogaNode);
		const float top = YGNodeLayoutGetTop(yogaNode) - parentScroll;
		const float borderLeft = YGNodeLayoutGetBorder(yogaNode, YGEdgeLeft);

		float parentLeft = 0.f;
		float parentTop = 0.f;

		auto parent = node->Parent;
		if (parent)
		{
			parentLeft = parent->ComputedPosition.x;
			parentTop = parent->ComputedPosition.y;
		}

		node->ComputedSize = Vector2(width, height);
		node->ComputedPosition = Vector2(left + parentLeft, top + parentTop);

		transform = glm::translate(transform, Vector3(left + parentLeft, top + parentTop, z));
		transform = glm::scale(transform, Vector3(width, height, 1.0f));

		mShader->Bind();
		mShader->SetUniforms({
			{ "u_Model",		transform },
			{ "u_Size",			Vector2(width, height) },
			{ "u_View",			mView },
			{ "u_Color",		node->ComputedStyle.BackgroundColor },
			{ "u_Border",		node->ComputedStyle.BorderSize },
			{ "u_BorderRadius",		node->ComputedStyle.BorderRadius },
			{ "u_BorderColor",	node->ComputedStyle.BorderColor },
		});

		bool hideOverflow = node->Parent != nullptr && node->Parent->ComputedStyle.Overflow == OverflowType::Hidden;
		if (hideOverflow)
		{
			glEnable(GL_SCISSOR_TEST);
			Vector2 parentPosition = node->Parent->ComputedPosition;
			Vector2 parentSize = node->Parent->ComputedSize;
			int scissorX = (int)parentPosition.x;
			int scissorY = 1080 - (int)(parentPosition.y - parentSize.y);
			int scissorW = (int)parentSize.x;
			int scissorH = (int)parentSize.y;
			glScissor(scissorX, scissorY, scissorW, scissorH);
		}

		mVertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);		
		mVertexArray->Unbind();
		mShader->Unbind();

		if (hideOverflow)
			glDisable(GL_SCISSOR_TEST);

	}

	void Renderer::DrawString(const std::string& string, NodeStyle& nodeStyle, std::shared_ptr<Font> font, Vector3 position)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		const float fontSize = nodeStyle.FontSize / 64.f;

		mSDFShader->Bind();
		font->mAtlas->Bind(5);
		mSDFShader->SetUniforms({
			{ "u_View", mView},
			{"u_Atlas", 5},
			{"u_FontColor", nodeStyle.FontColor},
			{ "u_PxRange",   fontSize },
		});
		
		mVertexArray->Bind();
		
		float advance = 0.0f;
		for (char const& c : string)
		{
			Char letter = font->GetChar((int)c);

			// Move the cursor
			Matrix4 model = Matrix4(1.f);
			model = glm::translate(model, position);
			model = glm::translate(model, Vector3(advance * fontSize, (-(letter.PlaneBounds.top) + (font->LineHeight)) * fontSize, 0.f));

			advance += (letter.Advance);

			// Scaling of the quad
			float scaleX = (float)(letter.PlaneBounds.right - letter.PlaneBounds.left);
			float scaleY = (float)(letter.PlaneBounds.top - letter.PlaneBounds.bottom);
			model = glm::scale(model, Vector3(scaleX * fontSize, scaleY * fontSize, 0.f));

			// Set Uniforms
			mSDFShader->SetUniforms({
				{ "u_Model", model },
				{ "u_TexturePos",   Vector2(letter.AtlasBounds.Pos.x, letter.AtlasBounds.Pos.y) },
				{ "u_TextureScale", Vector2(letter.AtlasBounds.Size.x, letter.AtlasBounds.Size.y) },
			});

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		mVertexArray->Unbind();
	}
}