#include "EditorUtils.h"
#include <imgui.h>

namespace Hexy 
{
	namespace Editor 
	{
		static int propertyId = 0;
		static char propertyIdText[16];

		static int activeGrids = 0;

		void BeginGrid(int width)
		{
			ImGui::Columns(width);
			propertyId = 0;
			activeGrids++;
		}

		void EndGrid()
		{
			ImGui::Columns(1);
			if (activeGrids > 0)
			{
				activeGrids--;
			}
		}

		bool Property(const std::string label, const std::string& value) 
		{
			if (label.empty())
			{
				ImGui::PushItemWidth(-1);
			}
			else 
			{
				ImGui::Text(label.c_str());
				if (activeGrids > 0) 
				{
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
				}
				else 
				{
					ImGui::SameLine();
				}
			}

			propertyIdText[0] = '#';
			propertyIdText[1] = '#';
			sprintf_s(propertyIdText + 2, 14, "%x", propertyId++);

			char buffer[256];
			strcpy_s(buffer, value.c_str());

			ImGui::InputText(propertyIdText, buffer, 256, ImGuiInputTextFlags_ReadOnly);

			if (label.empty()) {
				ImGui::PopItemWidth();
			}
			else if (activeGrids > 0) {
				ImGui::NextColumn();
				ImGui::PopItemWidth();
			}
			return false;
		}

		bool Property(const std::string label, std::string& value, bool error)
		{
			if (label.empty()) 
			{
				ImGui::PushItemWidth(-1);
			}
			else
			{
				ImGui::Text(label.c_str());
				if (activeGrids > 0) 
				{
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
				}
				else 
				{
					ImGui::SameLine();
				}
			}

			propertyIdText[0] = '#';
			propertyIdText[1] = '#';
			sprintf_s(propertyIdText + 2, 14, "%x", propertyId++);

			char buffer[256];
			strcpy_s(buffer, value.c_str());

			bool modified = false;

			if (error)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));

			if (ImGui::InputText(propertyIdText, buffer, 256)) {
				value = buffer;
				modified = true;
			}

			if (label.empty())
			{
				ImGui::PopItemWidth();
			}
			else if (activeGrids > 0) 
			{
				ImGui::NextColumn();
				ImGui::PopItemWidth();
			}

			if (error)
				ImGui::PopStyleColor();

			return modified;
		}

		bool Property(const std::string label, glm::vec3& vec)
		{
			if (label.empty())
			{
				ImGui::PushItemWidth(-1);
			}
			else
			{
				ImGui::Text(label.c_str());
				if (activeGrids > 0)
				{
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
				}
				else 
				{
					ImGui::SameLine();
				}
			}

			propertyIdText[0] = '#';
			propertyIdText[1] = '#';
			sprintf_s(propertyIdText+2, 14, "%x", propertyId++);

			bool modified = false;

			if (ImGui::DragFloat3(propertyIdText, glm::value_ptr(vec))) 
			{
				modified = true;
			}

			if (label.empty())
			{
				ImGui::PopItemWidth();
			}
			else if (activeGrids > 0)
			{
				ImGui::NextColumn();
				ImGui::PopItemWidth();
			}
			return modified;
		}

	}
}