#pragma once
#include "Canvas.h"
#include "Text.h"

#include <memory>

#include <imgui.h>
#include <Dependencies/NuakeRenderer/NuakeRenderer/NuakeRenderer.h>

namespace NuakeUI
{
	std::shared_ptr<Node> mSelectedNode;

	void DrawUI(std::shared_ptr<Node> node)
	{
		ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | 
			ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		// Highlight the selected node using a flag.
		if (mSelectedNode == node)
			base_flags |= ImGuiTreeNodeFlags_Selected;

		// Appends the classes of the node next to the name
		// Logic is only add [] if theres is a class and only add commans in between.
		std::string nodeTitle = node->GetID();
		const int classAmount = node->Classes.size();
		if (classAmount > 0)
		{
			nodeTitle += "[";
			for (int i = 0; i < classAmount; i++)
			{
				nodeTitle += node->Classes[i];

				if (i < classAmount - 1)
					nodeTitle += ", ";
			}
			nodeTitle += "]";
		}
		
		const bool nodeOpen = ImGui::TreeNodeEx(nodeTitle.c_str(), base_flags);

		// Select the if clicked
		if (ImGui::IsItemClicked())
		{
			mSelectedNode = node;
		}

		// Draw the rest of th nodes recursively.
		if (nodeOpen)
		{
			for (auto& c : node->GetChildrens())
				DrawUI(c);

			ImGui::TreePop();
		}
	}

	static void DrawNodeEditor()
	{
		if (!mSelectedNode)
		{
			ImGui::Text("No node selected.");
			return;
		}

		auto type = mSelectedNode->GetType();

		ImGui::ColorEdit4("Background Color", (float*) &mSelectedNode->Style.background_color);
		ImGui::Separator();
		ImGui::DragFloat("Border Size", &mSelectedNode->Style.border, 1.f, 0.f);
		ImGui::ColorEdit4("Border Color", (float*)&mSelectedNode->Style.border_color);

		if (type == NodeType::Text)
		{
			auto text = std::reinterpret_pointer_cast<Text>(mSelectedNode);
			ImGui::DragFloat("Font Size", &text->FontStyle.FontSize, 1.f, 0.f);
			ImGui::ColorEdit4("Font Color", (float*)&text->FontStyle.FontColor);
			ImGui::ColorEdit4("Font Background Color", (float*)&text->FontStyle.BackgroundColor);
		}
	}

	static void DrawInspector(std::shared_ptr<Canvas> canvas)
	{
		NuakeRenderer::BeginImGuiFrame();

		ImGui::ShowDemoWindow();

		if (ImGui::Begin("Inspector"))
		{
			if (ImGui::BeginTabBar("MyTabBar"))
			{
				if (ImGui::BeginTabItem("Tree"))
				{
					const float treeWidth = ImGui::GetWindowContentRegionWidth();
					const float availHeight = ImGui::GetContentRegionAvail().y;
					const ImVec2 size = ImVec2(treeWidth * 0.33f, availHeight);
					const ImVec2 size2 = ImVec2(treeWidth * 0.66f, availHeight);
					if (ImGui::BeginChild("Tree", size))
					{
						DrawUI(canvas->GetRootNode());
					}
					ImGui::EndChild();

					ImGui::SameLine();

					if (ImGui::BeginChild("Editor", size2))
					{
						DrawNodeEditor();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Canvas"))
				{
					
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			
		}
		ImGui::End();

		NuakeRenderer::EndImGuiFrame();
	}
}