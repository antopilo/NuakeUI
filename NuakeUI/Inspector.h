#pragma once
#include "Canvas.h"
#include "Text.h"
#include "StyleSheet.h"

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

		ImGui::ColorEdit4("Background Color", (float*) &mSelectedNode->ComputedStyle.BackgroundColor);
		ImGui::Separator();
		ImGui::DragFloat("Border Size", &mSelectedNode->ComputedStyle.BorderSize, 1.f, 0.f);
		ImGui::ColorEdit4("Border Color", (float*)&mSelectedNode->ComputedStyle.BorderColor);

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
				if (ImGui::BeginTabItem("StyleSheet"))
				{
					auto styleSheet = canvas->GetStyleSheet();

					for (auto& r : styleSheet->Rules)
					{
						std::string imguiText = "";
						for(int s = 0; s < r.Selector.size(); s++)
						{ 
							std::string selectorText = "";

							auto& selector = r.Selector[s];
							auto type = selector.Type;
							if (type == StyleSelectorType::Id)
								selectorText += "#";
							else if (type == StyleSelectorType::Class)
								selectorText += ".";

							selectorText += selector.Value.c_str();

							if(s < r.Selector.size() - 1)
								selectorText += ", ";

							imguiText += selectorText;
						}

						imguiText += " { ";
						ImGui::Text(imguiText.c_str());

						ImGui::Indent(8.f);

						// Now the properties!
						for (auto& rule : r.Properties)
						{
							// Name
							
							StyleProperties type = rule.first;
							if (type == StyleProperties::Width) ImGui::Text("Width: ");
							else if (type == StyleProperties::Height) ImGui::Text("Height: ");
							else if (type == StyleProperties::MinWidth) ImGui::Text("min-width: ");
							else if (type == StyleProperties::MinHeight) ImGui::Text("min-height: ");
							else if (type == StyleProperties::MaxWidth) ImGui::Text("max-width: ");
							else if (type == StyleProperties::MaxHeight) ImGui::Text("max-height: ");

							ImGui::SameLine();

							std::string valueText = "";
							// value
							PropValue value = rule.second;
							if (value.type == PropValueType::Percent)
							{
								valueText += std::to_string(value.value.Number);
								valueText += "%;";
							}
							else if (value.type == PropValueType::Pixel)
							{
								valueText += std::to_string(value.value.Number);
								valueText += "px;";
							}
							else if (value.type == PropValueType::Color)
							{

							}
							else if (value.type == PropValueType::Auto)
							{
								valueText += "auto;";
							}

							ImGui::Text(valueText.c_str());
						}
						ImGui::Indent(-8.f);
						ImGui::Text("}");
					}

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

		NuakeRenderer::EndImGuiFrame();
	}
}