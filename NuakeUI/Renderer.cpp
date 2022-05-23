#include "Renderer.h"
#include "NuakeRenderer/Math.h"
#include <NuakeRenderer/NuakeRenderer.h>
#include <NuakeRenderer/VertexBufferLayout.h>
#include "FileSystem.h"

namespace NuakeUI
{
	struct Vertex {
		Vector3 Position;
		Vector2 UV;
	};

	Renderer::Renderer()
	{
		std::string vertexSource = FileSystem::ReadFile("../resources/panel.vert.glsl");
		std::string fragSource = FileSystem::ReadFile("../resources/panel.frag.glsl");
		mShader = std::make_shared<NuakeRenderer::Shader>(vertexSource, fragSource);

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
		mView = glm::ortho(0.f, size.x, size.y, 0.f);
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
}